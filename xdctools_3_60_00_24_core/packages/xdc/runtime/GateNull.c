/* 
 *  Copyright (c) 2008 Texas Instruments. All rights reserved. 
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
 *  ======== GateNull.c ========
 */

#include "package/internal/GateNull.xdc.h"

/*
 *  ======== enter ========
 */
IArg GateNull_enter(GateNull_Object *obj)
{
    return (1);
}

/*
 *  ======== leave ========
 */
Void GateNull_leave(GateNull_Object *obj, IArg key)
{
}

/*
 *  ======== query ========
 */
/* REQ_TAG(SYSBIOS-927) */
Bool GateNull_query(Int qual)
{
    return (FALSE);
}
/*
 *  @(#) xdc.runtime; 2, 1, 0,0; 8-21-2019 13:22:46; /db/ztree/library/trees/xdc/xdc-H25/src/packages/
 */

