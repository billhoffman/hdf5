/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at the  *
 * root level of an installed copy of the electronic HDF5 document set and   *
 * is linked from the top-level documents page.  It can also be found at     *
 * http://hdfgroup.org/HDF5/doc/Copyright.html.  If you do not have          *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * Module Info: This module contains the functionality for setting & querying
 *      the datatype byte order for the H5T interface.
 */

#define H5T_PACKAGE		/*suppress error about including H5Tpkg	  */

/* Interface initialization */
#define H5_INTERFACE_INIT_FUNC	H5T_init_order_interface


#include "H5private.h"		/* Generic Functions			*/
#include "H5Eprivate.h"		/* Error handling		  	*/
#include "H5Iprivate.h"		/* IDs			  		*/
#include "H5Tpkg.h"		/* Datatypes				*/


/*--------------------------------------------------------------------------
NAME
   H5T_init_order_interface -- Initialize interface-specific information
USAGE
    herr_t H5T_init_order_interface()

RETURNS
    Non-negative on success/Negative on failure
DESCRIPTION
    Initializes any interface-specific data or routines.  (Just calls
    H5T_init_iterface currently).

--------------------------------------------------------------------------*/
static herr_t
H5T_init_order_interface(void)
{
    FUNC_ENTER_NOAPI_NOINIT_NOFUNC(H5T_init_order_interface)

    FUNC_LEAVE_NOAPI(H5T_init())
} /* H5T_init_order_interface() */


/*-------------------------------------------------------------------------
 * Function:	H5Tget_order
 *
 * Purpose:	Returns the byte order of a datatype.
 *
 * Return:	Success:	A byte order constant
 *
 *		Failure:	H5T_ORDER_ERROR (Negative)
 *
 * Programmer:	Robb Matzke
 *		Wednesday, January  7, 1998
s
 *
 *-------------------------------------------------------------------------
 */
H5T_order_t
H5Tget_order(hid_t type_id)
{
    H5T_t		*dt;
    H5T_order_t		ret_value;

    FUNC_ENTER_API(H5Tget_order, H5T_ORDER_ERROR)
    H5TRACE1("To", "i", type_id);

    /* Check args */
    if(NULL == (dt = H5I_object_verify(type_id,H5I_DATATYPE)))
	HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, H5T_ORDER_ERROR, "not a datatype")

    /* Get order */
    if((ret_value = H5T_get_order(dt)) == H5T_ORDER_ERROR)
	HGOTO_ERROR(H5E_DATATYPE, H5E_UNSUPPORTED, H5T_ORDER_ERROR, "cant't get order for specified datatype")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Tget_order() */


/*-------------------------------------------------------------------------
 * Function:	H5T_get_order
 *
 * Purpose:	Returns the byte order of a datatype.
 *
 * Return:	Success:	A byte order constant
 *		Failure:	H5T_ORDER_ERROR (Negative)
 *
 * Programmer:	Quincey Koziol
 *		Wednesday, October 17, 2007
 *
 *-------------------------------------------------------------------------
 */
H5T_order_t
H5T_get_order(const H5T_t *dt)
{
    H5T_order_t		ret_value;      /* Return value */

    FUNC_ENTER_NOAPI(H5T_get_order, H5T_ORDER_ERROR)

    /*defer to parent*/
    while(dt->shared->parent)
        dt = dt->shared->parent;
    if(!H5T_IS_ATOMIC(dt->shared))
	HGOTO_ERROR(H5E_DATATYPE, H5E_UNSUPPORTED, H5T_ORDER_ERROR, "operation not defined for specified datatype")

    /* Order */
    ret_value = dt->shared->u.atomic.order;

done:
    FUNC_LEAVE_NOAPI(ret_value)
} /* end H5T_get_order() */


/*-------------------------------------------------------------------------
 * Function:	H5Tset_order
 *
 * Purpose:	Sets the byte order for a datatype.
 *
 * Return:	Non-negative on success/Negative on failure
 *
 * Programmer:	Robb Matzke
 *		Wednesday, January  7, 1998
 *
 * Modifications:
 * 	Robb Matzke, 22 Dec 1998
 *	Also works for derived datatypes.
 *
 *      Raymond Lu, 18 Aug 2010
 *      Now it works for all data types with some restrictions:
 *        1. For enum type, members shouldn't be defined yet.
 *        2. H5T_ORDER_NONE only works for reference and fixed-length 
 *           string.
 *        3. For opaque type, the order will be ignored.
 *        4. For compound type, all restrictions above apply to the 
 *           members.
 *-------------------------------------------------------------------------
 */
herr_t
H5Tset_order(hid_t type_id, H5T_order_t order)
{
    H5T_t	*dt = NULL;
    herr_t      ret_value=SUCCEED;       /* Return value */

    FUNC_ENTER_API(H5Tset_order, FAIL)
    H5TRACE2("e", "iTo", type_id, order);

    /* Check args */
    if (NULL == (dt = H5I_object_verify(type_id,H5I_DATATYPE)))
	HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "not a datatype")
    if (order < H5T_ORDER_LE || order > H5T_ORDER_NONE)
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "illegal byte order")
    if (H5T_STATE_TRANSIENT!=dt->shared->state)
	HGOTO_ERROR(H5E_ARGS, H5E_CANTINIT, FAIL, "datatype is read-only")

    if (H5T_set_order(dt, order) < 0)
	HGOTO_ERROR(H5E_DATATYPE, H5E_UNSUPPORTED, FAIL, "can't set order")

done:
    FUNC_LEAVE_API(ret_value)
}


/*-------------------------------------------------------------------------
 * Function:	H5T_set_order
 *
 * Purpose:	Private function to set the byte order for a datatype.
 *
 * Return:	Non-negative on success/Negative on failure
 *
 * Programmer:	Raymond Lu
 *		13 August 2010
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5T_set_order(H5T_t *dtype, H5T_order_t order)
{
    H5T_t       *memb_type;         /* Datatype of compound members */
    int         nmemb;             /* Number of members in compound & enum types */
    int         i;                  /* Local index variable */
    herr_t      ret_value=SUCCEED;  /* Return value */

    FUNC_ENTER_NOAPI(H5T_set_order, FAIL)

    if (H5T_ENUM==dtype->shared->type && dtype->shared->u.enumer.nmembs>0)
	HGOTO_ERROR(H5E_DATATYPE, H5E_CANTINIT, FAIL, "operation not allowed after enum members are defined")

    /* For derived data type, defer to parent */ 
    while (dtype->shared->parent)
        dtype = dtype->shared->parent;

    if (order == H5T_ORDER_NONE && !(H5T_REFERENCE == dtype->shared->type || 
            H5T_OPAQUE == dtype->shared->type || H5T_IS_FIXED_STRING(dtype->shared)))
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "illegal byte order")

    /* For atomic data type */
    if (H5T_IS_ATOMIC(dtype->shared)) {
        dtype->shared->u.atomic.order = order;
        HGOTO_DONE(ret_value)
    }

    /* Loop through all fields of compound type */
    if(H5T_COMPOUND == dtype->shared->type) {
	if((nmemb = H5T_get_nmembers(dtype)) < 0)
	    HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "can't get number of members from compound data type")

	/* Set order for each compound member type. */
	for(i=0; i<nmemb; i++) {
	    if(H5T_set_order(dtype->shared->u.compnd.memb[i].type, order) < 0)
	        HGOTO_ERROR(H5E_DATATYPE, H5E_UNSUPPORTED, FAIL, "can't set order for compound member")
	}
    }

done:
    FUNC_LEAVE_NOAPI(ret_value)
}
