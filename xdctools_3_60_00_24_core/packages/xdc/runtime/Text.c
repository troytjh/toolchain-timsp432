/* 
 *  Copyright (c) 2008-2019 Texas Instruments Incorporated
 *  This program and the accompanying materials are made available under the
 *  terms of the Eclipse Public License v1.0 and Eclipse Distribution License
 *  v. 1.0 which accompanies this distribution. The Eclipse Public License is
 *  available at http://www.eclipse.org/legal/epl-v10.html and the Eclipse
 *  Distribution License is available at
 *  http://www.eclipse.org/org/documents/edl-v10.php.
 *
 *  Contributors:
 *      Texas Instruments - initial implementation
 * */
/*
 *  ======== Text.c ========
 */

#include <xdc/runtime/System.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Registry.h>

#include <string.h>
#include <stdarg.h>

#include "package/internal/Text.xdc.h"

/*
 *  ======== cordText ========
 *  This function is invoked from Core_assignLabel, which is invoked from
 *  Mod_Handle_label. The purpose of the function is to signal to
 *  Core_assignLabel that an instance name is in charTab, but charTab is not
 *  loaded to the target, or if that's not the case then the argument is simply
 *  returned back.
 */
String Text_cordText(Text_CordAddr cord)
{
    Char *p = (Char *)cord;

    /* REQ_TAG(SYSBIOS-888) */
    if (p >= Text_charTab && p < Text_charTab + Text_charCnt
        && Text_isLoaded == FALSE) {
        return (NULL);
    }

    return ((String)cord);
}

/*
 *  ======== matchRope ========
 */
/* REQ_TAG(SYSBIOS-893) */
Int Text_matchRope(Types_RopeId rope, CString pat, UShort *lenp)
{
    Text_MatchVisState state;

    state.pat = pat;
    state.lenp = lenp;

    /*
     * Text_visitRopeFxn is a config parameter that references a function
     * generated by the template Text.xdt and initialized in Text.xdc.
     * It's visitRope() with a stack large enough for all ropes in the
     * application.
     */
    Text_visitRopeFxn(rope, Text_matchVisFxn, &state);

    return (state.res);
}

/*
 *  ======== matchVisFxn ========
 *  Determines whether a given pattern matches the given string.
 *
 *  'obj' is a pointer a MatchVisState structure.
 *    pat - The pattern string to match to, which may contain the wildcard '%'
 *    lenp - The length of the pattern string
 *    res - Used to hold the result of this comparison
 *
 *  'src' is the string to compare the pattern to. It is intended to be a
 *  single node within a rope.
 *
 *  This function is intended for matching ropes one segment at a time. The
 *  return value indicates whether the comparison is complete--it returns
 *  FALSE if we need to move on to the next rope node, or TRUE if the
 *  comparison is complete. The actual result of the comparison is returned
 *  through the MatchVisState. 'res' is 1 if it's a match, -1 if not.
 *
 *  If *lenp == 0 and *src == '\0' at the same time, state->res will stay at 0
 *  if the calling function doesn't call again with the new 'src'. If it does,
 *  *src != '\0' and state->res changes to -1.
 */
Bool Text_matchVisFxn(Ptr obj, CString src)
{
    Text_MatchVisState *state;
    CString pat;
    UShort *lenp;

    state = obj;
    pat = state->pat;
    lenp = state->lenp;

    /* compare pat to src and return if pat != src or wildcard matches */
    for (; (*lenp != 0U) && (*src != '\0'); *lenp -= 1U) {
        if (*src != *pat) {
            state->res = *pat == '%' ? 1 : -1;
            return (TRUE);
        }
        src++;
        pat++;
    }

    /* if src[0] != 0, we reached the end of pat, there is no wildcard at there
     * end, and we haven't finished the string, so there is no match. If the
     * pattern is 'xdc.runtime.Timestamp' and the string is
     * 'xdc.runtime.TimestampNull', that's not a match.
     */
    if (*src != '\0') {
        state->res = -1;
        return (TRUE);
    }
    else {
        /* we reached end of src, we need to get next part of rope */
        state->res = 0;
        state->pat = pat;
        return (FALSE);
    }
}

/*
 *  ======== printVisFxn ========
 */
Bool Text_printVisFxn(Ptr obj, CString src)
{
    Text_PrintVisState *state = obj;

    if (state->len == 0U) {
        return (TRUE);
    }
    else {
        Int oc = Text_xprintf(state->bufp, state->len, "%s", src);
        state->res += oc;
        state->len -= (UShort)oc;
        return (FALSE);
    }
}

/*
 *  ======== putLab ========
 *  len == -1 => infinite output
 */
/* REQ_TAG(SYSBIOS-895), REQ_TAG(SYSBIOS-897), REQ_TAG(SYSBIOS-898) */
Int Text_putLab(Types_Label *lab, Char **bufp, Int len)
{
    Int res;

    res = Text_putMod(lab->modId, bufp, len);
    /* need at most 10 characters for "%p" + '\0'*/
    if (len < 0 || (len - res) > (8 + 1)) {
        /* -1 conversion to UInt is well defined so -1-res will result in a
         * practically infinite output.
         */
        res += Text_xprintf(bufp, (UInt)len - (UInt)res, "%p", lab->handle);
    }

    if (lab->named == TRUE
        && (len < 0 || (len - res) >= (5 + (Int)strlen(lab->iname))) ) {
        res += Text_xprintf(bufp, (UInt)len - (UInt)res, "('%s')", lab->iname);
    }

    return (res);
}

/*
 *  ======== putMod ========
 */
/* REQ_TAG(SYSBIOS-894), REQ_TAG(SYSBIOS-897), REQ_TAG(SYSBIOS-898) */
Int Text_putMod(Types_ModuleId mid, Char **bufp, Int len)
{
    Text_PrintVisState state;

    /* If this is an unnamed module... */
    if (mid <= Text_unnamedModsLastId) {
        return (Text_xprintf(bufp, (SizeT)len, "{module#%d}", mid));
    }

    /* If this is a dynamically registered module...
     * Modules with the ID between Text_unnamedModsLastId and
     * Text_registryModsLastId can exist only if some code outside of
     * xdc.runtime called Registry_addModule at runtime. Metacode for such
     * runtime code is obligated to call useModule('Registry') at the config
     * time. By this change, we will possibly break some code that did not
     * follow this rule before.
     */
    if (mid <= Text_registryModsLastId) {
        Registry_Desc *desc = Registry_findById(mid);
        CString fmt = desc != NULL ? desc->modName : "{module#%d}";
        return (Text_xprintf(bufp, (SizeT)len, fmt, mid));
    }

    /* This is a static, named module, and we know that the strings are loaded.
     * Otherwise, the module's Id wouldn't be above unnamedModsLastId.
     */
    state.bufp = bufp;
    state.len = len < 0 ? (UShort)0x7fff : (UShort)len;
        /* 0x7fff == infinite, almost */
    state.res = 0;
    Text_visitRopeFxn(mid, Text_printVisFxn, &state);

    return (state.res);
}

/*
 *  ======== putSite ========
 *  len == -1 => infinite output
 *
 *  If site->mod == 0, the module is unspecified and will be omitted from
 *  the output.
 */
/* REQ_TAG(SYSBIOS-896), REQ_TAG(SYSBIOS-897), REQ_TAG(SYSBIOS-898) */
Int Text_putSite(Types_Site *site, Char **bufp, Int len)
{
    UShort res;
    UShort max = (len < 0 || len > 0x7fff) ? 0x7fffU : (UShort)len;
    /* 0x7fff == infinite, well almost */

    res = 0;

    if (site == NULL) {
        return (0);
    }

    /* The 'mod' field is optional; 0 if it's unspecified. */
    if (site->mod != 0U) {
        res = (UShort)Text_putMod(site->mod, bufp, (Int)max);
    }

    if ((max - res) > 0U) {
        /* Don't output this if there's no mod */
        if (site->mod != 0U) {
            res += (UShort)Text_xprintf(bufp, (UInt)max - (UInt)res, ": ");
        }

        if (site->line == 0) {
            return ((Int)res);
        }

        if (site->file != (CString)NULL
            && ((UShort)(max - res) >= (strlen(site->file) + 5U))) {
            res += (UShort)
                Text_xprintf(bufp, (UInt)max - (UInt)res, "\"%s\", ",
                             site->file);
        }

        /* 7 + 1 = length of "line : " including '\0', 10 = max decimal digits
           in 32-bit number */
        if ((max - res) >= (8U + 10U)) {
            res += (UShort)
                Text_xprintf(bufp, (UInt)max - (UInt)res, "line %d: ",
                             site->line);
        }
    }

    return ((Int)res);
}

/*
 *  ======== ropeText ========
 *  All calls outside of this module are made only if charTab is on the target.
 *  Internal calls are from visitRope2, and that function is invoked from
 *  visitRope only when charTab is on the target.
 */
CString Text_ropeText(Text_RopeId rope)
{
    return ((rope & 0x8000U) != 0U ? (CString)NULL : Text_charTab + rope);
}

/*
 *  ======== visitRope2 ========
 *  Call visFxn on each "part" of the rope until visFxn returns TRUE or we
 *  reach the end of our rope.
 *
 *  The stack array must be large enough to hold the maximum number of
 *  nodes "in" rope.
 */
Void Text_visitRope2(Text_RopeId rope, Text_RopeVisitor visFxn, Ptr visState,
                     CString stack[])
{
    Int tos = 0;

    for (;;) {
        const Text_Node *node;
        UInt16 index;
        CString s = Text_ropeText(rope);
        if (s != NULL) {
            stack[tos] = s;
            tos++;
            break;
        }
        index = rope & 0x7fffU;
        node = Text_nodeTab + index;
        stack[tos] = Text_ropeText(node->right);
        tos++;
        rope = node->left;
    }

    do {
        CString s;
        tos--;
        s = stack[tos];
        if (visFxn(visState, s) != FALSE) {
            return;
        }
    } while (tos != 0);
}

/*
 *  ======== xprintf ========
 *  After xprintf returns, *bufp points to '\0'
 */
Int Text_xprintf(Char **bufp, SizeT len, CString fmt, ...)
{
    va_list va;
    Char *b;
    SizeT res;

    (void)va_start(va, fmt);
    b = (bufp != NULL && *bufp != NULL) ? *bufp : (Char*)NULL;

    res = (SizeT)System_vsnprintf(b, len, fmt, va);

    /*
     * vsnprintf returns num of chars that would have been written had `len`
     * been sufficiently large, but that number does not include a possible
     * '\0'. But, 'len' means "print no more than 'len' including '\0'". If
     * they are equal, the actual num of chars written would be `len` - 1.
     */
    if (res >= len) {
        res = len - 1U;
    }

    if (b != NULL) {
        /* the pointer points to '\0' if it was written */
        *bufp += res;
    }

    va_end(va);

    return ((Int)res);
}
/*
 *  @(#) xdc.runtime; 2, 1, 0,0; 8-21-2019 13:22:47; /db/ztree/library/trees/xdc/xdc-H25/src/packages/
 */

