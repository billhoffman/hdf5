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
 * Programmer:  Vailin Choi; Feb 2015
 */


/*
 * We include the private header file so we can get to the uniform
 * programming environment it declares.  
 * HDF5 API functions (except for H5G_basename())
 */
#include "H5private.h"
#include "h5tools.h"
#include "h5tools_utils.h"
#include "h5trav.h"

/* Name of tool */
#define PROGRAMNAME "h5format_convert"

static char *fname_g = NULL;
static char *dname_g = NULL;
static int dset_g = FALSE;
static int noop_g = FALSE;
static int verbose_g = 0;

/*
 * Command-line options: The user can specify short or long-named
 * parameters.
 */
static const char *s_opts = "hVvd:n";
static struct long_options l_opts[] = {
        { "help", no_arg, 'h' },
	{ "hel", no_arg, 'h'},
	{ "he", no_arg, 'h'},
        { "version", no_arg, 'V' },
	{ "version", no_arg, 'V' },
	{ "versio", no_arg, 'V' },
	{ "versi", no_arg, 'V' },
	{ "vers", no_arg, 'V' },
        { "verbose", no_arg, 'v' },
        { "verbos", no_arg, 'v' },
        { "verbo", no_arg, 'v' },
        { "verb", no_arg, 'v' },
        { "dname", require_arg, 'd' },
        { "dnam", require_arg, 'd' },
        { "dna", require_arg, 'd' },
        { "dn", require_arg, 'd' },
        { "noop", no_arg, 'n' },
        { "noo", no_arg, 'n' },
        { "no", no_arg, 'n' },
        { NULL, 0, '\0' }
};


/*-------------------------------------------------------------------------
 * Function: usage
 *
 * Purpose: print usage
 *
 * Return: void
 *
 *-------------------------------------------------------------------------
 */
static void usage(const char *prog) 
{
	printf("usage: %s [OPTIONS] file_name\n", prog);
	printf("  OPTIONS\n");
	printf("   -h, --help				Print a usage message and exit\n");
	printf("   -V, --version			Print version number and exit\n");
	printf("   -v, --verbose			Turn on verbose mode\n");
	printf("   -d dname, --dname=dataset_name	Pathname for the dataset\n");
	printf("   -n, --noop				Perform all the steps except the actual conversion\n");
	printf("\n");
	printf("Examples of use:\n");
	printf("\n");
	printf("h5format_convert -d /group/dataset file_name\n");
	printf("  Convert the chunk indexing type to version 1 B-tree\n");
	printf("  for the chunked dataset </group/dataset> in the HDF5 file <file_name>.\n");
	printf("\n");
	printf("h5format_convert file_name\n");
	printf("  Convert the chunk indexing type to version 1 B-tree\n");
	printf("  for all the chunked datasets in the HDF5 file <file_name>.\n");
	printf("\n");
	printf("h5format_convert -n -d /group/dataset file_name\n");
	printf("  Go through all the steps except the actual conversion when \n");
	printf("  converting the chunked dataset </group/dataset> in the HDF5 file <file_name>.\n");
} /* usage() */

/*-------------------------------------------------------------------------
 * Function: parse_command_line
 *
 * Purpose: parse command line input
 *
 * Return: Success: 0
 *  	   Failure: 1
 *
 *-------------------------------------------------------------------------
 */
static int
parse_command_line(int argc, const char **argv) 
{
    int opt;

     /* no arguments */
    if (argc == 1) {
        usage(h5tools_getprogname());
	h5tools_setstatus(EXIT_FAILURE);
        goto error;
    }

    /* parse command line options */
    while ((opt = get_option(argc, argv, s_opts, l_opts)) != EOF) {
	switch((char) opt) {
	    case 'h':
		usage(h5tools_getprogname());
		h5tools_setstatus(EXIT_SUCCESS);
		goto error;

	    case 'V':
		print_version(h5tools_getprogname());
		h5tools_setstatus(EXIT_SUCCESS);
		goto error;

	    case 'v':
		verbose_g = TRUE;
		break;

	    case 'd': /* -d dname */
		if(opt_arg != NULL && *opt_arg)
		/* if(opt_arg != NULL)*/
		    dname_g = HDstrdup(opt_arg);
		if(dname_g == NULL) {
		    h5tools_setstatus(EXIT_FAILURE);
		    error_msg("No dataset name\n", opt_arg);
		    usage(h5tools_getprogname());
		    goto error;
		}
		dset_g = TRUE;
		break;

	    case 'n': /* -n */
		noop_g = TRUE;
		break;

	    default:
		h5tools_setstatus(EXIT_FAILURE);
		usage(h5tools_getprogname());
		goto error;
		break;
	} /* switch */
    } /* while */

    if (argc <= opt_ind) {
        error_msg("missing file name\n");
        usage(h5tools_getprogname());
	h5tools_setstatus(EXIT_FAILURE);
        goto error;
    }

    fname_g = HDstrdup(argv[opt_ind]);

    return(0);

error:
    return(-1); ;
} /* parse_command_line() */


/*-------------------------------------------------------------------------
 * Function: leave
 *
 * Purpose: Close HDF5
 *
 * Return: Does not return
 *
 *-------------------------------------------------------------------------
 */
static void
leave(int ret)
{
    h5tools_close();

    HDexit(ret);
} /* leave() */

/*-------------------------------------------------------------------------
 * Function: convert()
 *
 * Purpose: To change the chunk indexing type for the dataset to version 1 B-tree.
 *	    -- the dataset has to be chunked
 *	    -- the dataset's chunk indexing type is not already version 1 B-tree.
 *	    If the above conditions are not fulfilled, the tool will not perform
 *	    the conversion but will exit with success.
 *
 * Return: Success: 0
 *  	   Failure: 1
 *
 *-------------------------------------------------------------------------
 */
static int
convert(hid_t fid, const char *dname)
{
    hid_t dcpl = -1;
    hid_t did = -1;
    H5D_layout_t layout_type;
    H5D_chunk_index_t idx_type;

    /* Open the dataset */
    if((did = H5Dopen2(fid, dname, H5P_DEFAULT)) < 0) {
	error_msg("unable to open dataset \"%s\"\n", dname);
	h5tools_setstatus(EXIT_FAILURE);
	goto error;

    } else if(verbose_g)
	printf("Open the dataset\n");

    /* Get the dataset's creation property list */
    if((dcpl = H5Dget_create_plist(did)) < 0) {
	error_msg("unable to get the dataset creation property list\n");
	h5tools_setstatus(EXIT_FAILURE);
	goto error;
    }

    /* Get the dataset's layout */
    if((layout_type = H5Pget_layout(dcpl)) < 0) {
	error_msg("unable to get the dataset layout type\n");
	h5tools_setstatus(EXIT_FAILURE);
	goto error;

    } else if(verbose_g)
	printf("Retrieve the dataset's layout\n");

    /* No further action if not a chunked dataset */
    if(layout_type != H5D_CHUNKED) {
	if(verbose_g)
	    printf("Dataset is not chunked: no further action\n");
	h5tools_setstatus(EXIT_SUCCESS);
	goto done;

    } else if(verbose_g)
	printf("Verify the dataset is a chunked dataset\n");

    /* Get the dataset's chunk indexing type */
    if(H5Dget_chunk_index_type(did, &idx_type) < 0) {
	error_msg("unable to get the chunk indexing type for \"%s\"\n", dname);
	h5tools_setstatus(EXIT_FAILURE);
	goto error;

    } else if(verbose_g)
	printf("Retrieve the dataset's chunk indexing type\n");

    /* No further action if the chunk indexing type is already version 1 B-tree */
    if(idx_type == H5D_CHUNK_IDX_BTREE) {
	if(verbose_g)
	    printf("Chunk indexing type is already version 1 B-tree: no further action\n");
	h5tools_setstatus(EXIT_SUCCESS);
	goto done;

    } else if (verbose_g)
	printf("Verify the dataset's chunk indexing type is not version 1 B-tree\n");

    /* No further action if it is a noop */
    if(noop_g) {
	if(verbose_g)
	    printf("Not converting the dataset\n");
	h5tools_setstatus(EXIT_SUCCESS);
	goto done;
    }

    if(verbose_g)
	printf("Converting the dataset...\n");

    /* Convert the dataset's chunk indexing type to version 1 B-tree */
    if(H5Dformat_convert(did) < 0) {
	error_msg("unable to convert chunk indexing for \"%s\"\n", dname);
	h5tools_setstatus(EXIT_FAILURE);
	goto error;

    } else if(verbose_g)
	printf("Done\n");

done:
    /* Close the dataset */
    if(H5Dclose(did) < 0) {
        error_msg("unable to close dataset \"%s\"\n", dname);
        h5tools_setstatus(EXIT_FAILURE);
	goto error;
    } else if(verbose_g)
	printf("Close the dataset\n");
    
    /* Close the dataset creation property list */
    if(H5Pclose(dcpl) < 0) {
        error_msg("unable to close dataset creation property list\n");
        h5tools_setstatus(EXIT_FAILURE);
	goto error;
    } else if(verbose_g)
	printf("Close the dataset creation property list\n");

    return(0);

error:
    if(verbose_g)
	printf("Error encountered\n");

    H5E_BEGIN_TRY {
        H5Pclose(dcpl);
        H5Dclose(did);
    } H5E_END_TRY;

     return(-1);

} /* convert() */

/*-------------------------------------------------------------------------
 * Function: convert_dsets_cb()
 *
 * Purpose:  The callback routine from the traversal to convert the
 *	     chunk indexing type of the dataset object.
 *
 * Return: Success: 0
 *  	   Failure: 1
 *-------------------------------------------------------------------------
 */
static int
convert_dsets_cb(const char *path, const H5O_info_t *oi, const char *already_visited, void *_fid)
{
    hid_t fid = *(hid_t *)_fid;

    /* If the object has already been seen then just return */
    if(NULL == already_visited) {

        if(oi->type == H5O_TYPE_DATASET) {
	    if(verbose_g)
		printf("Going to process dataset:%s...\n", path);
	    if(convert(fid, path) < 0)
		goto error;
	}

    } /* end if */

    return 0;

error:
    return -1;

} /* end convert_dsets_cb() */


/*-------------------------------------------------------------------------
 * Function: main
 *
 * Purpose: To convert the chunk indexing type of a dataset in a file to
 *	    version 1 B-tree.
 *
 * Return: Success: 0
 *  	   Failure: 1
 *
 *-------------------------------------------------------------------------
 */
int
main(int argc, const char *argv[])
{
    H5E_auto2_t func;
    void *edata;
    hid_t fid = -1;
    hid_t fcpl = -1;
    H5F_file_space_type_t strategy;
    hsize_t threshold;

    h5tools_setprogname(PROGRAMNAME);
    h5tools_setstatus(EXIT_SUCCESS);

    /* Disable error reporting */
    H5Eget_auto2(H5E_DEFAULT, &func, &edata);
    H5Eset_auto2(H5E_DEFAULT, NULL, NULL);

    /* Initialize h5tools lib */
    h5tools_init();

    /* Parse command line options */
    if(parse_command_line(argc, argv) < 0)
	goto done;
    else if(verbose_g)
	printf("Process command line options\n");

    if(noop_g && verbose_g)
	printf("It is noop...\n");

    /* Open the HDF5 file */
    if((fid = h5tools_fopen(fname_g, H5F_ACC_RDWR, H5P_DEFAULT, NULL, NULL, 0)) < 0) {
	error_msg("unable to open file \"%s\"\n", fname_g);
	h5tools_setstatus(EXIT_FAILURE);
	goto done;
    } else if(verbose_g)
	printf("Open the file %s\n", fname_g);

    if(dset_g) { /* Convert a specified dataset in the file */
	if(verbose_g)
	    printf("Going to process dataset: %s...\n", dname_g);
	if(convert(fid, dname_g) < 0)
	    goto done;
    } else { /* Convert all datasets in the file */
	if(verbose_g)
	    printf("Processing all datasets in the file...\n");
	if(h5trav_visit(fid, "/", TRUE, TRUE, convert_dsets_cb, NULL, &fid) < 0)
	    goto done;
    }

    if(verbose_g) {
        if(noop_g) {
            printf("Not processing the file's superblock...\n");
            h5tools_setstatus(EXIT_SUCCESS);
            goto done;
        }
        printf("Processing the file's superblock...\n");
    }

    if(H5Fformat_convert(fid) < 0) {
        error_msg("unable to convert file's superblock\"%s\"\n", fname_g);
        h5tools_setstatus(EXIT_FAILURE);
        goto done;
    }

done:
    /* Close the file */
    if(fid >= 0) {
	if(H5Fclose(fid) < 0) {
	    error_msg("unable to close file \"%s\"\n", fname_g);
	    h5tools_setstatus(EXIT_FAILURE);
	} else if(verbose_g)
	    printf("Close the file\n");
    } 

    if(fname_g)
	HDfree(fname_g);
    if(dname_g)
	HDfree(dname_g);
    
    H5Eset_auto2(H5E_DEFAULT, func, edata);
    leave(h5tools_getstatus());

} /* end main() */