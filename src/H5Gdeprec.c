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

/*-------------------------------------------------------------------------
 *
 * Created:	H5Gdeprec.c
 *		June 21 2006
 *		James Laird <jlaird@ncsa.uiuc.edu>
 *
 * Purpose:	Deprecated functions from the H5G interface.  These
 *              functions are here for compatibility purposes and may be
 *              removed in the future.  Applications should switch to the
 *              newer APIs.
 *
 *-------------------------------------------------------------------------
 */

/****************/
/* Module Setup */
/****************/

#define H5G_PACKAGE		/*suppress error about including H5Gpkg   */

/* Interface initialization */
#define H5_INTERFACE_INIT_FUNC	H5G__init_deprec_interface


/***********/
/* Headers */
/***********/
#include "H5private.h"		/* Generic Functions			*/
#include "H5ACprivate.h"	/* Metadata cache			*/
#include "H5Eprivate.h"		/* Error handling		  	*/
#include "H5Gpkg.h"		/* Groups		  		*/
#include "H5Iprivate.h"		/* IDs			  		*/
#include "H5Lprivate.h"         /* Links                                */
#include "H5Pprivate.h"         /* Property lists                       */
#include "H5VLprivate.h"	/* VOL plugins				*/

/****************/
/* Local Macros */
/****************/


/******************/
/* Local Typedefs */
/******************/

#ifndef H5_NO_DEPRECATED_SYMBOLS
/* User data for path traversal routine for getting object info */
typedef struct {
    H5G_stat_t  *statbuf;			/* Stat buffer about object */
    hbool_t follow_link;                        /* Whether we are following a link or not */
    H5F_t *loc_file;                            /* Pointer to the file the location is in */
    hid_t dxpl_id;                              /* Dataset transfer property list */
} H5G_trav_goi_t;


/********************/
/* Package Typedefs */
/********************/


/********************/
/* Local Prototypes */
/********************/

static herr_t H5G_get_objinfo_cb(H5G_loc_t *grp_loc/*in*/, const char *name,
    const H5O_link_t *lnk, H5G_loc_t *obj_loc, void *_udata/*in,out*/,
    H5G_own_loc_t *own_loc/*out*/);
static herr_t H5G_get_objinfo(const H5G_loc_t *loc, const char *name,
    hbool_t follow_link, H5G_stat_t *statbuf/*out*/, hid_t dxpl_id);
static H5G_obj_t H5G_obj_get_type_by_idx(H5O_loc_t *oloc, hsize_t idx,
    hid_t dxpl_id);
#endif /* H5_NO_DEPRECATED_SYMBOLS */


/*********************/
/* Package Variables */
/*********************/


/*****************************/
/* Library Private Variables */
/*****************************/


/*******************/
/* Local Variables */
/*******************/



/*--------------------------------------------------------------------------
NAME
   H5G__init_deprec_interface -- Initialize interface-specific information
USAGE
    herr_t H5G__init_deprec_interface()
RETURNS
    Non-negative on success/Negative on failure
DESCRIPTION
    Initializes any interface-specific data or routines.  (Just calls
    H5G__init() currently).

--------------------------------------------------------------------------*/
static herr_t
H5G__init_deprec_interface(void)
{
    FUNC_ENTER_STATIC_NOERR

    FUNC_LEAVE_NOAPI(H5G__init())
} /* H5G__init_deprec_interface() */


/*--------------------------------------------------------------------------
NAME
   H5G__term_deprec_interface -- Terminate interface
USAGE
    herr_t H5G__term_deprec_interface()
RETURNS
    Non-negative on success/Negative on failure
DESCRIPTION
    Terminates interface.  (Just resets H5_interface_initialize_g
    currently).

--------------------------------------------------------------------------*/
herr_t
H5G__term_deprec_interface(void)
{
    FUNC_ENTER_PACKAGE_NOERR

    /* Mark closed */
    H5_interface_initialize_g = 0;

    FUNC_LEAVE_NOAPI(0)
} /* H5G__term_deprec_interface() */

#ifndef H5_NO_DEPRECATED_SYMBOLS

/*-------------------------------------------------------------------------
 * Function:	H5G_map_obj_type
 *
 * Purpose:	Maps the object type to the older "group" object type
 *
 * Return:	Object type (can't fail)
 *
 * Programmer:	Quincey Koziol
 *              Tuesday, November 21, 2006
 *
 *-------------------------------------------------------------------------
 */
H5G_obj_t
H5G_map_obj_type(H5O_type_t obj_type)
{
    H5G_obj_t ret_value;        /* Return value */

    /* Use FUNC_ENTER_NOAPI_NOINIT_NOERR here to avoid performance issues */
    FUNC_ENTER_NOAPI_NOINIT_NOERR

    /* Map object type to older "group" object type */
    switch(obj_type) {
        case H5O_TYPE_GROUP:
            ret_value = H5G_GROUP;
            break;

        case H5O_TYPE_DATASET:
            ret_value = H5G_DATASET;
            break;

        case H5O_TYPE_NAMED_DATATYPE:
            ret_value = H5G_TYPE;
            break;

        case H5O_TYPE_UNKNOWN:
        case H5O_TYPE_NTYPES:
        default:
            ret_value = H5G_UNKNOWN;
    } /* end switch */

    FUNC_LEAVE_NOAPI(ret_value)
} /* end H5G_map_obj_type() */


/*-------------------------------------------------------------------------
 * Function:	H5Gcreate1
 *
 * Purpose:	Creates a new group relative to LOC_ID and gives it the
 *		specified NAME.  The group is opened for write access
 *		and it's object ID is returned.
 *
 *		The optional SIZE_HINT specifies how much file space to
 *		reserve to store the names that will appear in this
 *		group. If a non-positive value is supplied for the SIZE_HINT
 *		then a default size is chosen.
 *
 * Note:	Deprecated in favor of H5Gcreate2
 *
 * Return:	Success:	The object ID of a new, empty group open for
 *				writing.  Call H5Gclose() when finished with
 *				the group.
 *
 *		Failure:	FAIL
 *
 * Programmer:	Robb Matzke
 *		Wednesday, September 24, 1997
 *
 *-------------------------------------------------------------------------
 */
hid_t
H5Gcreate1(hid_t loc_id, const char *name, size_t size_hint)
{
    void    *grp = NULL;       /* dset token from VOL plugin */
    void    *obj = NULL;        /* object token of loc_id */
    H5VL_t  *vol_plugin;        /* VOL plugin information */
    H5VL_loc_params_t loc_params;
    hid_t           tmp_gcpl = (-1);    /* Temporary group creation property list */
    hid_t           lcpl_id = H5P_LINK_CREATE_DEFAULT;
    H5P_genplist_t  *plist;
    hid_t	    ret_value;          /* Return value */

    FUNC_ENTER_API(FAIL)
    H5TRACE3("i", "i*sz", loc_id, name, size_hint);

    /* Check arguments */
    if(!name || !*name)
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no name given")

    /* Check if we need to create a non-standard GCPL */
    if(size_hint > 0) {
        H5O_ginfo_t     ginfo;          /* Group info property */
        H5P_genplist_t  *gc_plist;      /* Property list created */

        /* Get the default property list */
        if(NULL == (gc_plist = (H5P_genplist_t *)H5I_object(H5P_GROUP_CREATE_DEFAULT)))
            HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "not a property list")

        /* Make a copy of the default property list */
        if((tmp_gcpl = H5P_copy_plist(gc_plist, FALSE)) < 0)
            HGOTO_ERROR(H5E_SYM, H5E_CANTGET, FAIL, "unable to copy the creation property list")

        /* Get pointer to the copied property list */
        if(NULL == (gc_plist = (H5P_genplist_t *)H5I_object(tmp_gcpl)))
            HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "not a property list")

        /* Get the group info property */
        if(H5P_get(gc_plist, H5G_CRT_GROUP_INFO_NAME, &ginfo) < 0)
            HGOTO_ERROR(H5E_PLIST, H5E_CANTGET, FAIL, "can't get group info")

        /* Set the non-default local heap size hint */
        ginfo.lheap_size_hint = size_hint;
        if(H5P_set(gc_plist, H5G_CRT_GROUP_INFO_NAME, &ginfo) < 0)
            HGOTO_ERROR(H5E_PLIST, H5E_CANTSET, FAIL, "can't set group info")
    } /* end if */
    else
        tmp_gcpl = H5P_GROUP_CREATE_DEFAULT;

    /* Get the plist structure */
    if(NULL == (plist = (H5P_genplist_t *)H5I_object(tmp_gcpl)))
        HGOTO_ERROR(H5E_ATOM, H5E_BADATOM, FAIL, "can't find object for ID")

    /* get creation properties */
    if(H5P_set(plist, H5VL_PROP_GRP_LCPL_ID, &lcpl_id) < 0)
        HGOTO_ERROR(H5E_PLIST, H5E_CANTGET, FAIL, "can't get property value for lcpl id")

    loc_params.type = H5VL_OBJECT_BY_SELF;
    loc_params.obj_type = H5I_get_type(loc_id);

    /* get the file object */
    if(NULL == (obj = (void *)H5I_object(loc_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid file identifier")

    /* get the plugin pointer */
    if (NULL == (vol_plugin = (H5VL_t *)H5I_get_aux(loc_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "ID does not contain VOL information")

    /* Create the group through the VOL */
    if(NULL == (grp = H5VL_group_create(obj, loc_params, vol_plugin, name, tmp_gcpl, 
                                        H5P_GROUP_ACCESS_DEFAULT, H5AC_dxpl_id, H5_REQUEST_NULL)))
	HGOTO_ERROR(H5E_SYM, H5E_CANTINIT, FAIL, "unable to create group")

    /* Get an atom for the group */
    if((ret_value = H5I_register2(H5I_GROUP, grp, vol_plugin, TRUE)) < 0)
	HGOTO_ERROR(H5E_ATOM, H5E_CANTREGISTER, FAIL, "unable to atomize dataset handle")

done:
    if(tmp_gcpl > 0 && tmp_gcpl != H5P_GROUP_CREATE_DEFAULT)
        if(H5I_dec_ref(tmp_gcpl) < 0)
            HDONE_ERROR(H5E_SYM, H5E_CLOSEERROR, FAIL, "unable to release property list")
    if (ret_value < 0 && grp)
        if(H5VL_group_close (grp, vol_plugin, H5AC_dxpl_id, H5_REQUEST_NULL) < 0)
            HDONE_ERROR(H5E_SYM, H5E_CLOSEERROR, FAIL, "unable to release group")

    FUNC_LEAVE_API(ret_value)
} /* end H5Gcreate1() */


/*-------------------------------------------------------------------------
 * Function:	H5Gopen1
 *
 * Purpose:	Opens an existing group for modification.  When finished,
 *		call H5Gclose() to close it and release resources.
 *
 * Note:	Deprecated in favor of H5Gopen2
 *
 * Return:	Success:	Object ID of the group.
 *
 *		Failure:	FAIL
 *
 * Programmer:	Robb Matzke
 *		Wednesday, December 31, 1997
 *
 *-------------------------------------------------------------------------
 */
hid_t
H5Gopen1(hid_t loc_id, const char *name)
{
    void    *grp = NULL;       /* dset token from VOL plugin */
    void    *obj = NULL;        /* object token of loc_id */
    H5VL_t  *vol_plugin;        /* VOL plugin information */
    H5VL_loc_params_t loc_params;
    hid_t       ret_value;              /* Return value */

    FUNC_ENTER_API(FAIL)
    H5TRACE2("i", "i*s", loc_id, name);

    /* Check args */
    if(!name || !*name)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no name")

    loc_params.type = H5VL_OBJECT_BY_SELF;
    loc_params.obj_type = H5I_get_type(loc_id);

    /* get the file object */
    if(NULL == (obj = (void *)H5I_object(loc_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid file identifier")

    /* get the plugin pointer */
    if (NULL == (vol_plugin = (H5VL_t *)H5I_get_aux(loc_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "ID does not contain VOL information")

    /* Create the group through the VOL */
    if(NULL == (grp = H5VL_group_open(obj, loc_params, vol_plugin, name, H5P_DEFAULT, 
                                      H5AC_dxpl_id, H5_REQUEST_NULL)))
	HGOTO_ERROR(H5E_SYM, H5E_CANTINIT, FAIL, "unable to create group")

    /* Get an atom for the group */
    if((ret_value = H5I_register2(H5I_GROUP, grp, vol_plugin, TRUE)) < 0)
	HGOTO_ERROR(H5E_ATOM, H5E_CANTREGISTER, FAIL, "unable to atomize dataset handle")

done:
    if (ret_value < 0 && grp)
        if(H5VL_group_close (grp, vol_plugin, H5AC_dxpl_id, H5_REQUEST_NULL) < 0)
            HDONE_ERROR(H5E_SYM, H5E_CLOSEERROR, FAIL, "unable to release group")
    FUNC_LEAVE_API(ret_value)
} /* end H5Gopen1() */


/*-------------------------------------------------------------------------
 * Function:	H5Glink
 *
 * Purpose:	Creates a link between two existing objects.  The new
 *              APIs to do this are H5Lcreate_hard and H5Lcreate_soft.
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Glink(hid_t cur_loc_id, H5G_link_t type, const char *cur_name, const char *new_name)
{
    hid_t  lcpl_id   = H5P_LINK_CREATE_DEFAULT;
    H5P_genplist_t *plist;      /* Property list pointer */
    herr_t ret_value = SUCCEED;         /* Return value */

    FUNC_ENTER_API(FAIL)
    H5TRACE4("e", "iLl*s*s", cur_loc_id, type, cur_name, new_name);

    /* Check arguments */
    if(!cur_name || !*cur_name)
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no current name specified")
    if(!new_name || !*new_name)
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no new name specified")

    /* Get the plist structure */
    if(NULL == (plist = (H5P_genplist_t *)H5I_object(lcpl_id)))
        HGOTO_ERROR(H5E_ATOM, H5E_BADATOM, FAIL, "can't find object for ID")

    if(type == H5L_TYPE_HARD) {
        void    *obj = NULL;        /* object token of loc_id */
        H5VL_t  *vol_plugin = NULL; /* VOL plugin information */
        H5VL_loc_params_t loc_params1;
        H5VL_loc_params_t loc_params2;

        loc_params1.type = H5VL_OBJECT_BY_NAME;
        loc_params1.obj_type = H5I_get_type(cur_loc_id);
        loc_params1.loc_data.loc_by_name.name = cur_name;
        loc_params1.loc_data.loc_by_name.lapl_id = H5P_DEFAULT;

        loc_params2.type = H5VL_OBJECT_BY_NAME;
        loc_params2.loc_data.loc_by_name.name = new_name;
        loc_params2.loc_data.loc_by_name.lapl_id = H5P_DEFAULT;

        /* get the file object */
        if(NULL == (obj = (void *)H5I_object(cur_loc_id)))
            HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid file identifier")
        /* get the plugin pointer */
        if (NULL == (vol_plugin = (H5VL_t *)H5I_get_aux(cur_loc_id)))
            HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "ID does not contain VOL information")

        /* set creation properties */
        if(H5P_set(plist, H5VL_PROP_LINK_TARGET, &obj) < 0)
            HGOTO_ERROR(H5E_PLIST, H5E_CANTGET, FAIL, "can't set property value for target")
        if(H5P_set(plist, H5VL_PROP_LINK_TARGET_LOC_PARAMS, &loc_params1) < 0)
            HGOTO_ERROR(H5E_PLIST, H5E_CANTGET, FAIL, "can't set property value for target loc params")

        /* Create the link through the VOL */
        if((ret_value = H5VL_link_create(H5VL_LINK_CREATE_HARD, NULL, loc_params2, vol_plugin,
                                         lcpl_id, H5P_DEFAULT, H5AC_dxpl_id, H5_REQUEST_NULL)) < 0)
            HGOTO_ERROR(H5E_SYM, H5E_CANTINIT, FAIL, "unable to create link")
    } /* end if */
    else if(type == H5L_TYPE_SOFT) {
        void    *obj = NULL;        /* object token of loc_id */
        H5VL_t  *vol_plugin = NULL; /* VOL plugin information */
        H5VL_loc_params_t loc_params;

        loc_params.type = H5VL_OBJECT_BY_NAME;
        loc_params.loc_data.loc_by_name.name = new_name;
        loc_params.loc_data.loc_by_name.lapl_id = H5P_DEFAULT;
        loc_params.obj_type = H5I_get_type(cur_loc_id);

        /* get the file object */
        if(NULL == (obj = (void *)H5I_object(cur_loc_id)))
            HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid file identifier")
        /* get the plugin pointer */
        if (NULL == (vol_plugin = (H5VL_t *)H5I_get_aux(cur_loc_id)))
            HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "ID does not contain VOL information")

        /* set creation properties */
        if(H5P_set(plist, H5VL_PROP_LINK_TARGET_NAME, &cur_name) < 0)
            HGOTO_ERROR(H5E_PLIST, H5E_CANTGET, FAIL, "can't get property value for target name")

        /* Create the link through the VOL */
        if((ret_value = H5VL_link_create(H5VL_LINK_CREATE_SOFT, obj, loc_params, vol_plugin, 
                                         lcpl_id, H5P_DEFAULT, H5AC_dxpl_id, H5_REQUEST_NULL)) < 0)
            HGOTO_ERROR(H5E_SYM, H5E_CANTINIT, FAIL, "unable to create link")
    } /* end else if */
    else
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "Not a valid link type")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Glink() */


/*-------------------------------------------------------------------------
 * Function:	H5Glink2
 *
 * Purpose:	Creates a link between two existing objects.  The new
 *              APIs to do this are H5Lcreate_hard and H5Lcreate_soft.
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Glink2(hid_t cur_loc_id, const char *cur_name, H5G_link_t type,
    hid_t new_loc_id, const char *new_name)
{
    hid_t  lcpl_id   = H5P_LINK_CREATE_DEFAULT;
    H5P_genplist_t *plist;      /* Property list pointer */
    herr_t ret_value = SUCCEED;         /* Return value */

    FUNC_ENTER_API(FAIL)
    H5TRACE5("e", "i*sLli*s", cur_loc_id, cur_name, type, new_loc_id, new_name);

    /* Check arguments */
    if(!cur_name || !*cur_name)
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no current name specified")
    if(!new_name || !*new_name)
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no new name specified")

    /* Get the plist structure */
    if(NULL == (plist = (H5P_genplist_t *)H5I_object(lcpl_id)))
        HGOTO_ERROR(H5E_ATOM, H5E_BADATOM, FAIL, "can't find object for ID")

    if(type == H5L_TYPE_HARD) {
        void    *obj1 = NULL;        /* object token of loc_id */
        void    *obj2 = NULL;        /* object token of loc_id */
        H5VL_t  *vol_plugin1 = NULL; /* VOL plugin information */
        H5VL_t  *vol_plugin2 = NULL; /* VOL plugin information */
        H5VL_loc_params_t loc_params1;
        H5VL_loc_params_t loc_params2;

        loc_params1.type = H5VL_OBJECT_BY_NAME;
        loc_params1.obj_type = H5I_get_type(cur_loc_id);
        loc_params1.loc_data.loc_by_name.name = cur_name;
        loc_params1.loc_data.loc_by_name.lapl_id = H5P_DEFAULT;

        loc_params2.type = H5VL_OBJECT_BY_NAME;
        loc_params2.obj_type = H5I_get_type(new_loc_id);
        loc_params2.loc_data.loc_by_name.name = new_name;
        loc_params2.loc_data.loc_by_name.lapl_id = H5P_DEFAULT;

        /* get the file object */
        if(NULL == (obj1 = (void *)H5I_object(cur_loc_id)))
            HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid file identifier")
        /* get the plugin pointer */
        if (NULL == (vol_plugin1 = (H5VL_t *)H5I_get_aux(cur_loc_id)))
            HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "ID does not contain VOL information")

        /* get the file object */
        if(NULL == (obj2 = (void *)H5I_object(new_loc_id)))
            HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid file identifier")
        /* get the plugin pointer */
        if (NULL == (vol_plugin2 = (H5VL_t *)H5I_get_aux(new_loc_id)))
            HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "ID does not contain VOL information")

        /* set creation properties */
        if(H5P_set(plist, H5VL_PROP_LINK_TARGET, &obj1) < 0)
            HGOTO_ERROR(H5E_PLIST, H5E_CANTGET, FAIL, "can't set property value for target id")
        if(H5P_set(plist, H5VL_PROP_LINK_TARGET_LOC_PARAMS, &loc_params1) < 0)
            HGOTO_ERROR(H5E_PLIST, H5E_CANTGET, FAIL, "can't set property value for target name")

        /* Create the link through the VOL */
        if((ret_value = H5VL_link_create(H5VL_LINK_CREATE_HARD, obj2, loc_params2, 
                                         (vol_plugin1!=NULL ? vol_plugin1 : vol_plugin2),
                                         lcpl_id, H5P_DEFAULT, H5AC_dxpl_id, H5_REQUEST_NULL)) < 0)
            HGOTO_ERROR(H5E_SYM, H5E_CANTINIT, FAIL, "unable to create link")
    } /* end if */
    else if(type == H5L_TYPE_SOFT) {
        void    *obj = NULL;        /* object token of loc_id */
        H5VL_t  *vol_plugin;        /* VOL plugin information */
        H5VL_loc_params_t loc_params;

        /* Soft links only need one location, the new_loc_id, but it's possible that
         * new_loc_id is H5L_SAME_LOC */
        if(new_loc_id == H5L_SAME_LOC)
            new_loc_id = cur_loc_id;

        loc_params.type = H5VL_OBJECT_BY_NAME;
        loc_params.loc_data.loc_by_name.name = new_name;
        loc_params.loc_data.loc_by_name.lapl_id = H5P_DEFAULT;
        loc_params.obj_type = H5I_get_type(new_loc_id);

        /* get the file object */
        if(NULL == (obj = (void *)H5I_object(new_loc_id)))
            HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid file identifier")
        /* get the plugin pointer */
        if (NULL == (vol_plugin = (H5VL_t *)H5I_get_aux(new_loc_id)))
            HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "ID does not contain VOL information")

        /* set creation properties */
        if(H5P_set(plist, H5VL_PROP_LINK_TARGET_NAME, &cur_name) < 0)
            HGOTO_ERROR(H5E_PLIST, H5E_CANTGET, FAIL, "can't get property value for target name")

        /* Create the link through the VOL */
        if((ret_value = H5VL_link_create(H5VL_LINK_CREATE_SOFT, obj, loc_params, vol_plugin, 
                                         lcpl_id, H5P_DEFAULT, H5AC_dxpl_id, H5_REQUEST_NULL)) < 0)
            HGOTO_ERROR(H5E_SYM, H5E_CANTINIT, FAIL, "unable to create link")
    } /* end else if */
    else
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "not a valid link type")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Glink2() */


/*-------------------------------------------------------------------------
 * Function:	H5Gmove
 *
 * Purpose:	Moves and renames a link.  The new API to do this is H5Lmove.
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Gmove(hid_t src_loc_id, const char *src_name, const char *dst_name)
{
    void    *obj = NULL;        /* object token of loc_id */
    H5VL_t  *vol_plugin = NULL; /* VOL plugin information */
    H5VL_loc_params_t loc_params1;
    H5VL_loc_params_t loc_params2;
    herr_t      ret_value = SUCCEED;       /* Return value */

    FUNC_ENTER_API(FAIL)
    H5TRACE3("e", "i*s*s", src_loc_id, src_name, dst_name);

    loc_params1.type = H5VL_OBJECT_BY_NAME;
    loc_params1.obj_type = H5I_get_type(src_loc_id);
    loc_params1.loc_data.loc_by_name.name = src_name;
    loc_params1.loc_data.loc_by_name.lapl_id = H5P_DEFAULT;

    loc_params2.type = H5VL_OBJECT_BY_NAME;
    loc_params2.loc_data.loc_by_name.name = dst_name;
    loc_params2.loc_data.loc_by_name.lapl_id = H5P_DEFAULT;

    /* get the file object */
    if(NULL == (obj = (void *)H5I_object(src_loc_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid file identifier")
    /* get the plugin pointer */
    if (NULL == (vol_plugin = (H5VL_t *)H5I_get_aux(src_loc_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "ID does not contain VOL information")

    /* Create the link through the VOL */
    if((ret_value = H5VL_link_move(obj, loc_params1, NULL, loc_params2, vol_plugin,
                                   H5P_DEFAULT, H5P_DEFAULT, H5AC_dxpl_id, H5_REQUEST_NULL)) < 0)
	HGOTO_ERROR(H5E_SYM, H5E_CANTINIT, FAIL, "unable to create link")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Gmove() */


/*-------------------------------------------------------------------------
 * Function:	H5Gmove2
 *
 * Purpose:	Moves and renames a link.  The new API to do this is H5Lmove.
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Gmove2(hid_t src_loc_id, const char *src_name, hid_t dst_loc_id,
    const char *dst_name)
{
    void    *obj1 = NULL;        /* object token of src_id */
    H5VL_t  *vol_plugin1;        /* VOL plugin information */
    H5VL_loc_params_t loc_params1;
    void    *obj2 = NULL;        /* object token of dst_id */
    H5VL_t  *vol_plugin2;        /* VOL plugin information */
    H5VL_loc_params_t loc_params2;
    herr_t      ret_value = SUCCEED;       /* Return value */

    FUNC_ENTER_API(FAIL)
    H5TRACE4("e", "i*si*s", src_loc_id, src_name, dst_loc_id, dst_name);

    /* set location paramter for source object */
    loc_params1.type = H5VL_OBJECT_BY_NAME;
    loc_params1.loc_data.loc_by_name.name = src_name;
    loc_params1.loc_data.loc_by_name.lapl_id = H5P_DEFAULT;
    loc_params1.obj_type = H5I_get_type(src_loc_id);
    /* set location paramter for destination object */
    loc_params2.type = H5VL_OBJECT_BY_NAME;
    loc_params2.loc_data.loc_by_name.name = dst_name;
    loc_params2.loc_data.loc_by_name.lapl_id = H5P_DEFAULT;
    loc_params2.obj_type = H5I_get_type(dst_loc_id);

    if(H5L_SAME_LOC != src_loc_id) {
        /* get the file object */
        if(NULL == (obj1 = (void *)H5I_object(src_loc_id)))
            HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid file identifier")
        /* get the plugin pointer */
        if (NULL == (vol_plugin1 = (H5VL_t *)H5I_get_aux(src_loc_id)))
            HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "ID does not contain VOL information")
    }
    if(H5L_SAME_LOC != dst_loc_id) {
        /* get the file object */
        if(NULL == (obj2 = (void *)H5I_object(dst_loc_id)))
            HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid file identifier")
        /* get the plugin pointer */
        if (NULL == (vol_plugin2 = (H5VL_t *)H5I_get_aux(dst_loc_id)))
            HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "ID does not contain VOL information")
    }
    /* Make sure that the VOL plugins are the same */
    if(H5L_SAME_LOC != dst_loc_id && H5L_SAME_LOC != src_loc_id) {
        if (vol_plugin1 != vol_plugin2)
            HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "Objects are accessed through different VOL plugins and can't be linked")
    }

    /* Move the link through the VOL */
    if((ret_value = H5VL_link_move(obj1, loc_params1, obj2, loc_params2, 
                                   (vol_plugin1!=NULL ? vol_plugin1 : vol_plugin2), 
                                   H5P_DEFAULT, H5P_DEFAULT, H5AC_dxpl_id, H5_REQUEST_NULL)) < 0)
	HGOTO_ERROR(H5E_SYM, H5E_CANTINIT, FAIL, "unable to create link")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Gmove2() */


/*-------------------------------------------------------------------------
 * Function:	H5Gunlink
 *
 * Purpose:	Removes a link.  The new API is H5Ldelete/H5Ldelete_by_idx.
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Gunlink(hid_t loc_id, const char *name)
{
    void    *obj = NULL;        /* object token of loc_id */
    H5VL_t  *vol_plugin;        /* VOL plugin information */
    H5VL_loc_params_t loc_params;
    herr_t ret_value = SUCCEED;         /* Return value */

    FUNC_ENTER_API(FAIL)
    H5TRACE2("e", "i*s", loc_id, name);

    if(!name || !*name)
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no name")

    loc_params.type = H5VL_OBJECT_BY_NAME;
    loc_params.obj_type = H5I_get_type(loc_id);
    loc_params.loc_data.loc_by_name.name = name;
    loc_params.loc_data.loc_by_name.lapl_id = H5P_LINK_ACCESS_DEFAULT;

    /* get the file object */
    if(NULL == (obj = (void *)H5I_object(loc_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid file identifier")
    /* get the plugin pointer */
    if (NULL == (vol_plugin = (H5VL_t *)H5I_get_aux(loc_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "ID does not contain VOL information")

    /* Delete the link through the VOL */
    if(H5VL_link_specific(obj, loc_params, vol_plugin, H5VL_LINK_DELETE, 
                          H5AC_dxpl_id, H5_REQUEST_NULL) < 0)
	HGOTO_ERROR(H5E_SYM, H5E_CANTDELETE, FAIL, "unable to delete link")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Gunlink() */


/*-------------------------------------------------------------------------
 * Function:	H5Gget_linkval
 *
 * Purpose:	Retrieve's a soft link's data.  The new API is
 *              H5Lget_val/H5Lget_val_by_idx.
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Gget_linkval(hid_t loc_id, const char *name, size_t size, char *buf/*out*/)
{
    void    *obj = NULL;        /* object token of loc_id */
    H5VL_t  *vol_plugin;        /* VOL plugin information */
    H5VL_loc_params_t loc_params;
    herr_t ret_value = SUCCEED;         /* Return value */

    FUNC_ENTER_API(FAIL)
    H5TRACE4("e", "i*szx", loc_id, name, size, buf);

    /* Check arguments */
    if(!name || !*name)
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no name specified")

    loc_params.type = H5VL_OBJECT_BY_NAME;
    loc_params.obj_type = H5I_get_type(loc_id);
    loc_params.loc_data.loc_by_name.name = name;
    loc_params.loc_data.loc_by_name.lapl_id = H5P_LINK_ACCESS_DEFAULT;

    /* get the file object */
    if(NULL == (obj = (void *)H5I_object(loc_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid file identifier")
    /* get the plugin pointer */
    if (NULL == (vol_plugin = (H5VL_t *)H5I_get_aux(loc_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "ID does not contain VOL information")

    /* Get the link info through the VOL */
    if((ret_value = H5VL_link_get(obj, loc_params, vol_plugin, H5VL_LINK_GET_VAL, 
                                  H5AC_ind_dxpl_id, H5_REQUEST_NULL, buf, size)) < 0)
        HGOTO_ERROR(H5E_INTERNAL, H5E_CANTGET, FAIL, "unable to get link value")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Gget_linkval() */


/*-------------------------------------------------------------------------
 * Function:	H5Gset_comment
 *
 * Purpose:     Gives the specified object a comment.  The COMMENT string
 *		should be a null terminated string.  An object can have only
 *		one comment at a time.  Passing NULL for the COMMENT argument
 *		will remove the comment property from the object.
 *
 * Note:	Deprecated in favor of H5Oset_comment/H5Oset_comment_by_name
 *
 * Return:	Non-negative on success/Negative on failure
 *
 * Programmer:	Robb Matzke
 *              Monday, July 20, 1998
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Gset_comment(hid_t loc_id, const char *name, const char *comment)
{
    H5G_loc_t	loc;
    herr_t      ret_value = SUCCEED;       /* Return value */

    FUNC_ENTER_API(FAIL)
    H5TRACE3("e", "i*s*s", loc_id, name, comment);

    if(H5G_loc(loc_id, &loc) < 0)
	HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "not a location")
    if(!name || !*name)
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no name specified")

    if(H5G_loc_set_comment(&loc, name, comment, H5P_DEFAULT, H5AC_dxpl_id) < 0)
	HGOTO_ERROR(H5E_SYM, H5E_CANTINIT, FAIL, "unable to set comment value")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Gset_comment() */


/*-------------------------------------------------------------------------
 * Function:	H5Gget_comment
 *
 * Purpose:	Return at most BUFSIZE characters of the comment for the
 *		specified object.  If BUFSIZE is large enough to hold the
 *		entire comment then the comment string will be null
 *		terminated, otherwise it will not.  If the object does not
 *		have a comment value then no bytes are copied to the BUF
 *		buffer.
 *
 * Note:	Deprecated in favor of H5Oget_comment/H5Oget_comment_by_name
 *
 * Return:	Success:	Number of characters in the comment counting
 *				the null terminator.  The value returned may
 *				be larger than the BUFSIZE argument.
 *
 *		Failure:	Negative
 *
 * Programmer:	Robb Matzke
 *              Monday, July 20, 1998
 *
 *-------------------------------------------------------------------------
 */
int
H5Gget_comment(hid_t loc_id, const char *name, size_t bufsize, char *buf)
{
    H5G_loc_t	loc;
    int	ret_value;

    FUNC_ENTER_API(FAIL)
    H5TRACE4("Is", "i*sz*s", loc_id, name, bufsize, buf);

    if(H5G_loc(loc_id, &loc) < 0)
	HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "not a location")
    if(!name || !*name)
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no name specified")
    if(bufsize > 0 && !buf)
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no buffer specified")

    if((ret_value = (int)H5G_loc_get_comment(&loc, name, buf, bufsize, H5P_DEFAULT, H5AC_ind_dxpl_id)) < 0)
	HGOTO_ERROR(H5E_SYM, H5E_CANTINIT, FAIL, "unable to get comment value")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Gget_comment() */

#if 0

/*-------------------------------------------------------------------------
 * Function:	H5Gset_comment
 *
 * Purpose:     Gives the specified object a comment.  The COMMENT string
 *		should be a null terminated string.  An object can have only
 *		one comment at a time.  Passing NULL for the COMMENT argument
 *		will remove the comment property from the object.
 *
 * Note:	Deprecated in favor of H5Oset_comment/H5Oset_comment_by_name
 *
 * Return:	Non-negative on success/Negative on failure
 *
 * Programmer:	Robb Matzke
 *              Monday, July 20, 1998
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Gset_comment(hid_t loc_id, const char *name, const char *comment)
{
    void    *obj = NULL;        /* object token of loc_id */
    H5VL_t  *vol_plugin;        /* VOL plugin information */
    H5VL_loc_params_t loc_params;
    herr_t      ret_value = SUCCEED;       /* Return value */

    FUNC_ENTER_API(FAIL)
    H5TRACE3("e", "i*s*s", loc_id, name, comment);

    if(!name || !*name)
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no name specified")

    loc_params.type = H5VL_OBJECT_BY_NAME;
    loc_params.loc_data.loc_by_name.name = name;
    loc_params.loc_data.loc_by_name.lapl_id = H5P_LINK_ACCESS_DEFAULT;
    loc_params.obj_type = H5I_get_type(loc_id);

    /* get the file object */
    if(NULL == (obj = (void *)H5I_object(loc_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid file identifier")
    /* get the plugin pointer */
    if (NULL == (vol_plugin = (H5VL_t *)H5I_get_aux(loc_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "ID does not contain VOL information")

    /* set comment on object through the VOL */
    if(H5VL_object_optional(obj, vol_plugin, H5AC_dxpl_id, H5_REQUEST_NULL, 
                            H5VL_OBJECT_SET_COMMENT, loc_params, comment) < 0)
	HGOTO_ERROR(H5E_SYM, H5E_CANTINIT, FAIL, "unable to set comment value")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Gset_comment() */


/*-------------------------------------------------------------------------
 * Function:	H5Gget_comment
 *
 * Purpose:	Return at most BUFSIZE characters of the comment for the
 *		specified object.  If BUFSIZE is large enough to hold the
 *		entire comment then the comment string will be null
 *		terminated, otherwise it will not.  If the object does not
 *		have a comment value then no bytes are copied to the BUF
 *		buffer.
 *
 * Note:	Deprecated in favor of H5Oget_comment/H5Oget_comment_by_name
 *
 * Return:	Success:	Number of characters in the comment counting
 *				the null terminator.  The value returned may
 *				be larger than the BUFSIZE argument.
 *
 *		Failure:	Negative
 *
 * Programmer:	Robb Matzke
 *              Monday, July 20, 1998
 *
 *-------------------------------------------------------------------------
 */
int
H5Gget_comment(hid_t loc_id, const char *name, size_t bufsize, char *buf)
{
    void    *obj = NULL;        /* object token of loc_id */
    H5VL_t  *vol_plugin;        /* VOL plugin information */
    H5VL_loc_params_t loc_params;
    ssize_t size;
    int	ret_value;

    FUNC_ENTER_API(FAIL)
    H5TRACE4("Is", "i*sz*s", loc_id, name, bufsize, buf);

    if(!name || !*name)
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no name specified")
    if(bufsize > 0 && !buf)
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no buffer specified")

    loc_params.type = H5VL_OBJECT_BY_NAME;
    loc_params.loc_data.loc_by_name.name = name;
    loc_params.loc_data.loc_by_name.lapl_id = H5P_LINK_ACCESS_DEFAULT;
    loc_params.obj_type = H5I_get_type(loc_id);

    /* get the file object */
    if(NULL == (obj = (void *)H5I_object(loc_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid file identifier")
    /* get the plugin pointer */
    if (NULL == (vol_plugin = (H5VL_t *)H5I_get_aux(loc_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "ID does not contain VOL information")

    if(H5VL_object_optional(obj, vol_plugin, H5AC_dxpl_id, H5_REQUEST_NULL, 
                            H5VL_OBJECT_GET_COMMENT, loc_params, buf, bufsize, &size) < 0)
        HGOTO_ERROR(H5E_INTERNAL, H5E_CANTGET, FAIL, "unable to get object comment")

    ret_value = (int)size;

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Gget_comment() */
#endif


/*-------------------------------------------------------------------------
 * Function:	H5Giterate
 *
 * Purpose:	Iterates over the entries of a group.  The LOC_ID and NAME
 *		identify the group over which to iterate and IDX indicates
 *		where to start iterating (zero means at the beginning).	 The
 *		OPERATOR is called for each member and the iteration
 *		continues until the operator returns non-zero or all members
 *		are processed. The operator is passed a group ID for the
 *		group being iterated, a member name, and OP_DATA for each
 *		member.
 *
 * Note:	Deprecated in favor of H5Literate
 *
 * Return:	Success:	The return value of the first operator that
 *				returns non-zero, or zero if all members were
 *				processed with no operator returning non-zero.
 *
 *		Failure:	Negative if something goes wrong within the
 *				library, or the negative value returned by one
 *				of the operators.
 *
 * Programmer:	Robb Matzke
 *		Monday, March 23, 1998
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Giterate(hid_t loc_id, const char *name, int *idx_p, H5G_iterate_t op,
    void *op_data)
{
    H5G_loc_t           loc;            /* Location of object */
    H5G_link_iterate_t lnk_op;          /* Link operator */
    hsize_t     last_obj;               /* Index of last object looked at */
    hsize_t	idx;                    /* Internal location to hold index */
    herr_t	ret_value;

    FUNC_ENTER_API(FAIL)
    H5TRACE5("e", "i*s*Isx*x", loc_id, name, idx_p, op, op_data);

    /* Check args */
    if(H5G_loc(loc_id, &loc) < 0)
	HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "not a location ID")
    if(!name || !*name)
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no name specified")
    if(idx_p && *idx_p < 0)
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "invalid index specified")
    if(!op)
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no operator specified")

    /* Set number of objects looked at to zero */
    last_obj = 0;
    idx = (hsize_t)(idx_p == NULL ? 0 : *idx_p);

    /* Build link operator info */
    lnk_op.op_type = H5G_LINK_OP_OLD;
    lnk_op.op_func.op_old = op;

    /* Call private function. */
    if((ret_value = H5G_iterate(&loc, name, H5_INDEX_NAME, H5_ITER_INC, idx, &last_obj, &lnk_op, op_data, H5P_DEFAULT, H5AC_ind_dxpl_id)) < 0)
	HGOTO_ERROR(H5E_SYM, H5E_BADITER, FAIL, "group iteration failed")

    /* Set the index we stopped at */
    if(idx_p)
        *idx_p = (int)last_obj;

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Giterate() */


/*-------------------------------------------------------------------------
 * Function:	H5Gget_num_objs
 *
 * Purpose:     Returns the number of objects in the group.  It iterates
 *              all B-tree leaves and sum up total number of group members.
 *
 * Note:	Deprecated in favor of H5Gget_info
 *
 * Return:	Success:        Non-negative
 *		Failure:	Negative
 *
 * Programmer:	Raymond Lu
 *	        Nov 20, 2002
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Gget_num_objs(hid_t loc_id, hsize_t *num_objs)
{
    H5G_loc_t		loc;            /* Location of object */
    H5G_info_t          grp_info;       /* Group information */
    H5O_type_t          obj_type;       /* Type of object at location */
    herr_t		ret_value = SUCCEED;

    FUNC_ENTER_API(FAIL)
    H5TRACE2("e", "i*h", loc_id, num_objs);

    /* Check args */
    if(H5G_loc(loc_id, &loc) < 0)
	HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "not a location ID")
    if(H5O_obj_type(loc.oloc, &obj_type, H5AC_ind_dxpl_id) < 0)
        HGOTO_ERROR(H5E_SYM, H5E_CANTGET, FAIL, "can't get object type")
    if(obj_type != H5O_TYPE_GROUP)
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "not a group")
    if(!num_objs)
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "bad pointer to # of objects")

    /* Retrieve information about the group */
    if(H5G__obj_info(loc.oloc, &grp_info, H5AC_ind_dxpl_id) < 0)
	HGOTO_ERROR(H5E_SYM, H5E_CANTCOUNT, FAIL, "can't determine")

    /* Set the number of objects [sic: links] in the group */
    *num_objs = grp_info.nlinks;

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Gget_num_objs() */


/*-------------------------------------------------------------------------
 * Function:	H5Gget_objinfo
 *
 * Purpose:	Returns information about an object.  If FOLLOW_LINK is
 *		non-zero then all symbolic links are followed; otherwise all
 *		links except the last component of the name are followed.
 *
 * Note:	Deprecated in favor of H5Lget_info/H5Oget_info
 *
 * Return:	Non-negative on success, with the fields of STATBUF (if
 *              non-null) initialized. Negative on failure.
 *
 * Programmer:	Robb Matzke
 *              Monday, April 13, 1998
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Gget_objinfo(hid_t loc_id, const char *name, hbool_t follow_link,
	       H5G_stat_t *statbuf/*out*/)
{
    H5G_loc_t	loc;
    herr_t      ret_value = SUCCEED;       /* Return value */

    FUNC_ENTER_API(FAIL)
    H5TRACE4("e", "i*sbx", loc_id, name, follow_link, statbuf);

    /* Check arguments */
    if(H5G_loc(loc_id, &loc) < 0)
	HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "not a location")
    if(!name || !*name)
	HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no name specified")

    /* Get info */
    if(H5G_get_objinfo(&loc, name, follow_link, statbuf, H5AC_ind_dxpl_id) < 0)
	HGOTO_ERROR(H5E_ARGS, H5E_CANTINIT, FAIL, "cannot stat object")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Gget_objinfo() */


/*-------------------------------------------------------------------------
 * Function:	H5G_get_objinfo_cb
 *
 * Purpose:	Callback for retrieving info about an object.  This routine
 *              gets the info
 *
 * Return:	Non-negative on success/Negative on failure
 *
 * Programmer:	Quincey Koziol
 *              Tuesday, September 20, 2005
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5G_get_objinfo_cb(H5G_loc_t UNUSED *grp_loc/*in*/, const char *name, const H5O_link_t *lnk,
    H5G_loc_t *obj_loc, void *_udata/*in,out*/, H5G_own_loc_t *own_loc/*out*/)
{
    H5G_trav_goi_t *udata = (H5G_trav_goi_t *)_udata;   /* User data passed in */
    herr_t ret_value = SUCCEED;         /* Return value */

    FUNC_ENTER_NOAPI_NOINIT

    /* Check if the name in this group resolved to a valid link */
    if(lnk == NULL && obj_loc == NULL)
        HGOTO_ERROR(H5E_SYM, H5E_NOTFOUND, FAIL, "'%s' doesn't exist", name)

    /* Only modify user's buffer if it's available */
    if(udata->statbuf) {
        H5G_stat_t *statbuf = udata->statbuf;   /* Convenience pointer for statbuf */

        /* Common code to retrieve the file's fileno */
        if(H5F_get_fileno((obj_loc ? obj_loc : grp_loc)->oloc->file, &statbuf->fileno[0]) < 0)
            HGOTO_ERROR(H5E_FILE, H5E_BADVALUE, FAIL, "unable to read fileno")

        /* Info for soft and UD links is gotten by H5L_get_info. If we have
         *      a hard link, follow it and get info on the object
         */
        if(udata->follow_link || !lnk || (lnk->type == H5L_TYPE_HARD)) {
            H5O_info_t oinfo;           /* Object information */

            /* Go retrieve the object information */
            /* (don't need index & heap info) */
            HDassert(obj_loc);
            if(H5O_get_info(obj_loc->oloc, udata->dxpl_id, FALSE, &oinfo) < 0)
                HGOTO_ERROR(H5E_OHDR, H5E_CANTGET, FAIL, "unable to get object info")

            /* Get mapped object type */
            statbuf->type = H5G_map_obj_type(oinfo.type);

	    /* Get object number (i.e. address) for object */
	    statbuf->objno[0] = (unsigned long)(oinfo.addr);
#if H5_SIZEOF_UINT64_T > H5_SIZEOF_LONG
	    statbuf->objno[1] = (unsigned long)(oinfo.addr >> 8 * sizeof(long));
#else
	    statbuf->objno[1] = 0;
#endif
            /* Get # of hard links pointing to object */
	    statbuf->nlink = oinfo.rc;

            /* Get modification time for object */
            statbuf->mtime = oinfo.ctime;

            /* Retrieve the object header information */
            statbuf->ohdr.size = oinfo.hdr.space.total;
            statbuf->ohdr.free = oinfo.hdr.space.free;
            statbuf->ohdr.nmesgs = oinfo.hdr.nmesgs;
            statbuf->ohdr.nchunks = oinfo.hdr.nchunks;
        } /* end if */
    } /* end if */

done:
    /* Indicate that this callback didn't take ownership of the group *
     * location for the object */
    *own_loc = H5G_OWN_NONE;

    FUNC_LEAVE_NOAPI(ret_value)
} /* end H5G_get_objinfo_cb() */


/*-------------------------------------------------------------------------
 * Function:	H5G_get_objinfo
 *
 * Purpose:	Returns information about an object.
 *
 * Return:	Success:	Non-negative with info about the object
 *				returned through STATBUF if it isn't the null
 *				pointer.
 *
 *		Failure:	Negative
 *
 * Programmer:	Robb Matzke
 *              Monday, April 13, 1998
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5G_get_objinfo(const H5G_loc_t *loc, const char *name, hbool_t follow_link,
    H5G_stat_t *statbuf/*out*/, hid_t dxpl_id)
{
    H5G_trav_goi_t udata;           /* User data for callback */
    herr_t      ret_value = SUCCEED;       /* Return value */

    FUNC_ENTER_NOAPI_NOINIT

    HDassert(loc);
    HDassert(name && *name);

    /* Reset stat buffer, if one was given */
    if(statbuf)
        HDmemset(statbuf, 0, sizeof(H5G_stat_t));

    /* Set up user data for retrieving information */
    udata.statbuf = statbuf;
    udata.follow_link = follow_link;
    udata.loc_file = loc->oloc->file;
    udata.dxpl_id = dxpl_id;

    /* Traverse the group hierarchy to locate the object to get info about */
    if(H5G_traverse(loc, name, (unsigned)(follow_link ? H5G_TARGET_NORMAL : H5G_TARGET_SLINK|H5G_TARGET_UDLINK),
            H5G_get_objinfo_cb, &udata, H5P_DEFAULT, dxpl_id) < 0)
        HGOTO_ERROR(H5E_SYM, H5E_EXISTS, FAIL, "name doesn't exist")

    /* If we're pointing at a soft or UD link, get the real link length and type */
    if(statbuf && follow_link == 0) {
        H5L_info_t linfo;           /* Link information buffer */
        herr_t ret;

        /* Get information about link to the object. If this fails, e.g.
         * because the object is ".", just treat the object as a hard link. */
        H5E_BEGIN_TRY {
            ret = H5L_get_info(loc, name, &linfo, H5P_DEFAULT, dxpl_id);
        } H5E_END_TRY

        if(ret >= 0 && linfo.type != H5L_TYPE_HARD) {
            statbuf->linklen = linfo.u.val_size;
            if(linfo.type == H5L_TYPE_SOFT)
                statbuf->type = H5G_LINK;
            else {  /* UD link. H5L_get_info checked for invalid link classes */
                HDassert(linfo.type >= H5L_TYPE_UD_MIN && linfo.type <= H5L_TYPE_MAX);
                statbuf->type = H5G_UDLINK;
            } /* end else */
        } /* end if */
    } /* end if */

done:
    FUNC_LEAVE_NOAPI(ret_value)
} /* end H5G_get_objinfo() */


/*-------------------------------------------------------------------------
 * Function:	H5Gget_objname_by_idx
 *
 * Purpose:     Returns the name of objects in the group by giving index.
 *              If `name' is non-NULL then write up to `size' bytes into that
 *              buffer and always return the length of the entry name.
 *              Otherwise `size' is ignored and the function does not store the name,
 *              just returning the number of characters required to store the name.
 *              If an error occurs then the buffer pointed to by `name' (NULL or non-NULL)
 *              is unchanged and the function returns a negative value.
 *              If a zero is returned for the name's length, then there is no name
 *              associated with the ID.
 *
 * Note:	Deprecated in favor of H5Lget_name_by_idx
 *
 * Return:	Success:        Non-negative
 *		Failure:	Negative
 *
 * Programmer:	Raymond Lu
 *	        Nov 20, 2002
 *
 *-------------------------------------------------------------------------
 */
ssize_t
H5Gget_objname_by_idx(hid_t loc_id, hsize_t idx, char *name, size_t size)
{
    H5G_loc_t		loc;            /* Object location */
    H5O_type_t          obj_type;       /* Type of object at location */
    ssize_t		ret_value;

    FUNC_ENTER_API(FAIL)
    H5TRACE4("Zs", "ih*sz", loc_id, idx, name, size);

    /* Check args */
    if(H5G_loc(loc_id, &loc) < 0)
	HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "not a location ID")
    if(H5O_obj_type(loc.oloc, &obj_type, H5AC_ind_dxpl_id) < 0)
        HGOTO_ERROR(H5E_DATASET, H5E_CANTGET, FAIL, "can't get object type")
    if(obj_type != H5O_TYPE_GROUP)
        HGOTO_ERROR(H5E_DATASET, H5E_BADTYPE, FAIL, "not a group")

    /* Call internal function */
    if((ret_value = H5G_obj_get_name_by_idx(loc.oloc, H5_INDEX_NAME, H5_ITER_INC, 
                                            idx, name, size, H5AC_ind_dxpl_id)) < 0)
	HGOTO_ERROR(H5E_SYM, H5E_BADTYPE, FAIL, "can't get object name")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Gget_objname_by_idx() */


/*-------------------------------------------------------------------------
 * Function:	H5Gget_objtype_by_idx
 *
 * Purpose:     Returns the type of objects in the group by giving index.
 *
 * Note:	Deprecated in favor of H5Lget_info/H5Oget_info
 *
 * Return:	Success:        H5G_GROUP(1), H5G_DATASET(2), H5G_TYPE(3)
 *		Failure:	H5G_UNKNOWN
 *
 * Programmer:	Raymond Lu
 *	        Nov 20, 2002
 *
 *-------------------------------------------------------------------------
 */
H5G_obj_t
H5Gget_objtype_by_idx(hid_t loc_id, hsize_t idx)
{
    H5G_loc_t		loc;            /* Object location */
    H5O_type_t          obj_type;       /* Type of object at location */
    H5G_obj_t		ret_value;

    FUNC_ENTER_API(H5G_UNKNOWN)
    H5TRACE2("Go", "ih", loc_id, idx);

    /* Check args */
    if(H5G_loc(loc_id, &loc) < 0)
	HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, H5G_UNKNOWN, "not a location ID")
    if(H5O_obj_type(loc.oloc, &obj_type, H5AC_ind_dxpl_id) < 0)
        HGOTO_ERROR(H5E_SYM, H5E_CANTGET, H5G_UNKNOWN, "can't get object type")
    if(obj_type != H5O_TYPE_GROUP)
        HGOTO_ERROR(H5E_SYM, H5E_BADTYPE, H5G_UNKNOWN, "not a group")

    /* Call internal function*/
    if((ret_value = H5G_obj_get_type_by_idx(loc.oloc, idx, H5AC_ind_dxpl_id)) == H5G_UNKNOWN)
	HGOTO_ERROR(H5E_SYM, H5E_BADTYPE, H5G_UNKNOWN, "can't get object type")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Gget_objtype_by_idx() */


/*-------------------------------------------------------------------------
 * Function:	H5G_obj_get_type_by_idx
 *
 * Purpose:     Private function for H5Gget_objtype_by_idx.
 *              Returns the type of objects in the group by giving index.
 *
 * Return:	Success:        H5G_GROUP(1), H5G_DATASET(2), H5G_TYPE(3)
 *
 *		Failure:	Negative
 *
 * Programmer:	Raymond Lu
 *	        Nov 20, 2002
 *
 *-------------------------------------------------------------------------
 */
static H5G_obj_t
H5G_obj_get_type_by_idx(H5O_loc_t *oloc, hsize_t idx, hid_t dxpl_id)
{
    H5O_linfo_t	linfo;		/* Link info message */
    htri_t linfo_exists;        /* Whether the link info message exists */
    H5G_obj_t ret_value;        /* Return value */

    FUNC_ENTER_NOAPI_TAG(dxpl_id, oloc->addr, H5G_UNKNOWN)

    /* Sanity check */
    HDassert(oloc);

    /* Attempt to get the link info for this group */
    if((linfo_exists = H5G__obj_get_linfo(oloc, &linfo, dxpl_id)) < 0)
        HGOTO_ERROR(H5E_SYM, H5E_CANTGET, H5G_UNKNOWN, "can't check for link info message")
    if(linfo_exists) {
        if(H5F_addr_defined(linfo.fheap_addr)) {
            /* Get the object's name from the dense link storage */
            if((ret_value = H5G__dense_get_type_by_idx(oloc->file, dxpl_id, &linfo, idx)) < 0)
                HGOTO_ERROR(H5E_SYM, H5E_NOTFOUND, H5G_UNKNOWN, "can't locate type")
        } /* end if */
        else {
            /* Get the object's type from the link messages */
            if((ret_value = H5G__compact_get_type_by_idx(oloc, dxpl_id, &linfo, idx)) < 0)
                HGOTO_ERROR(H5E_SYM, H5E_NOTFOUND, H5G_UNKNOWN, "can't locate type")
        } /* end else */
    } /* end if */
    else {
        /* Get the object's type from the symbol table */
        if((ret_value = H5G__stab_get_type_by_idx(oloc, idx, dxpl_id)) < 0)
            HGOTO_ERROR(H5E_SYM, H5E_NOTFOUND, H5G_UNKNOWN, "can't locate type")
    } /* end else */

done:
    FUNC_LEAVE_NOAPI_TAG(ret_value, H5G_UNKNOWN)
} /* end H5G_obj_get_type_by_idx() */
#endif /* H5_NO_DEPRECATED_SYMBOLS */

