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
 *  ======== Gate.c ========
 */
#include <xdc/std.h>

#include <xdc/runtime/System.h>

/* Actual implementation of the following requirements is in Gate__epilogue.h */
/* REQ_TAG(SYSBIOS-922), REQ_TAG(SYSBIOS-923), REQ_TAG(SYSBIOS-924) */
/* REQ_TAG(SYSBIOS-925), REQ_TAG(SYSBIOS-926) */
#include "package/internal/Gate.xdc.h"

/*
 *  ======== enterSystem ========
 *  Enter the "System" gate
 *
 *  This method enters the gate specified by
 *    1. System.common$.gate, if non-null, otherwise
 *    2. Defaults.common$.gate, if non-null, otherwise
 *    3. NULL is used with GateNull
 *
 *  See finalize() in package.xs for details.
 */
/* REQ_TAG(SYSBIOS-928) */
IArg Gate_enterSystem(Void)
{
    return (xdc_runtime_System_Module_GateProxy_enter(
        xdc_runtime_System_Module__gateObj__C));
}

/*
 *  ======== leaveSystem ========
 */
/* REQ_TAG(SYSBIOS-929) */
Void Gate_leaveSystem(IArg key)
{
    xdc_runtime_System_Module_GateProxy_leave(
        xdc_runtime_System_Module__gateObj__C, key);
}
/*
 *  @(#) xdc.runtime; 2, 1, 0,0; 8-21-2019 13:22:46; /db/ztree/library/trees/xdc/xdc-H25/src/packages/
 */

