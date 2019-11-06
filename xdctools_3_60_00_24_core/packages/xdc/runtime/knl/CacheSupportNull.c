/*
 *  ======== CacheSupportNull.c =======r
 */

#include <xdc/std.h>
#include <xdc/runtime/Error.h>

#include "package/internal/CacheSupportNull.xdc.h"

/*
 *  ======== CacheSupportNull_inv ========
 */
Bool CacheSupportNull_inv(Ptr blockPtr, SizeT byteCnt, Bool wait, 
    Error_Block *eb)
{
    return (TRUE);
}

/*
 *  ======== CacheSupportNull_wb ========
 */
Bool CacheSupportNull_wb(Ptr blockPtr, SizeT byteCnt, Bool wait, 
Error_Block *eb)
{
    return (TRUE);
}

/*
 *  ======== CacheSupportNull_wbInv ========
 */
Bool CacheSupportNull_wbInv(Ptr blockPtr, SizeT byteCnt, Bool wait, 
    Error_Block *eb)
{
    return (TRUE);
}

/*
 *  ======== CacheSupportNull_wait ========
 */
Bool CacheSupportNull_wait(Error_Block *eb)
{
    return (TRUE);
}

/*
 *  @(#) xdc.runtime.knl; 1, 0, 0,0; 8-21-2019 13:22:52; /db/ztree/library/trees/xdc/xdc-H25/src/packages/
 */

