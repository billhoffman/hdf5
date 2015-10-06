HDF5 "3_2_vds.h5" {
GROUP "/" {
   DATASET "vds_dset" {
      DATATYPE  H5T_STD_I32LE
      DATASPACE  SIMPLE { ( 6, 13, 19 ) / ( H5S_UNLIMITED, 13, 19 ) }
      STORAGE_LAYOUT {
         MAPPING 0 { 
            VIRTUAL {
               SELECTION REGULAR_HYPERSLAB { 
                  START (0,1,1)
                  STRIDE (1,1,1)
                  COUNT (1,1,1)
                  BLOCK (H5S_UNLIMITED,2,7)
               }
            }
            SOURCE {
               FILE "2_a.h5"
               DATASET "/source_dset"
               SELECTION REGULAR_HYPERSLAB { 
                  START (0,0,0)
                  STRIDE (1,1,1)
                  COUNT (1,1,1)
                  BLOCK (H5S_UNLIMITED,2,7)
               }
            }
         }
         MAPPING 1 { 
            VIRTUAL {
               SELECTION REGULAR_HYPERSLAB { 
                  START (0,4,0)
                  STRIDE (1,1,1)
                  COUNT (1,1,1)
                  BLOCK (H5S_UNLIMITED,4,7)
               }
            }
            SOURCE {
               FILE "2_b.h5"
               DATASET "/source_dset"
               SELECTION REGULAR_HYPERSLAB { 
                  START (0,0,0)
                  STRIDE (1,1,1)
                  COUNT (1,1,1)
                  BLOCK (H5S_UNLIMITED,4,7)
               }
            }
         }
         MAPPING 2 { 
            VIRTUAL {
               SELECTION REGULAR_HYPERSLAB { 
                  START (0,11,4)
                  STRIDE (1,1,1)
                  COUNT (1,1,1)
                  BLOCK (H5S_UNLIMITED,2,7)
               }
            }
            SOURCE {
               FILE "2_c.h5"
               DATASET "/source_dset"
               SELECTION REGULAR_HYPERSLAB { 
                  START (0,0,0)
                  STRIDE (1,1,1)
                  COUNT (1,1,1)
                  BLOCK (H5S_UNLIMITED,2,7)
               }
            }
         }
         MAPPING 3 { 
            VIRTUAL {
               SELECTION REGULAR_HYPERSLAB { 
                  START (0,1,9)
                  STRIDE (1,1,1)
                  COUNT (1,1,1)
                  BLOCK (H5S_UNLIMITED,5,7)
               }
            }
            SOURCE {
               FILE "2_d.h5"
               DATASET "/source_dset"
               SELECTION REGULAR_HYPERSLAB { 
                  START (0,0,0)
                  STRIDE (1,1,1)
                  COUNT (1,1,1)
                  BLOCK (H5S_UNLIMITED,5,7)
               }
            }
         }
         MAPPING 4 { 
            VIRTUAL {
               SELECTION REGULAR_HYPERSLAB { 
                  START (0,8,12)
                  STRIDE (1,1,1)
                  COUNT (1,1,1)
                  BLOCK (H5S_UNLIMITED,3,7)
               }
            }
            SOURCE {
               FILE "2_e.h5"
               DATASET "/source_dset"
               SELECTION REGULAR_HYPERSLAB { 
                  START (0,0,0)
                  STRIDE (1,1,1)
                  COUNT (1,1,1)
                  BLOCK (H5S_UNLIMITED,3,7)
               }
            }
         }
      }
      FILTERS {
         NONE
      }
      FILLVALUE {
         FILL_TIME H5D_FILL_TIME_IFSET
         VALUE  -9
      }
      ALLOCATION_TIME {
         H5D_ALLOC_TIME_INCR
      }
      DATA {
      (0,0,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9,
      (0,0,15): -9, -9, -9, -9,
      (0,1,0): -9, 10, 10, 10, 10, 10, 10, 10, -9, 40, 40, 40, 40, 40, 40,
      (0,1,15): 40, -9, -9, -9,
      (0,2,0): -9, 10, 10, 10, 10, 10, 10, 10, -9, 40, 40, 40, 40, 40, 40,
      (0,2,15): 40, -9, -9, -9,
      (0,3,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, 40, 40, 40, 40, 40, 40,
      (0,3,15): 40, -9, -9, -9,
      (0,4,0): 20, 20, 20, 20, 20, 20, 20, -9, -9, 40, 40, 40, 40, 40, 40,
      (0,4,15): 40, -9, -9, -9,
      (0,5,0): 20, 20, 20, 20, 20, 20, 20, -9, -9, 40, 40, 40, 40, 40, 40,
      (0,5,15): 40, -9, -9, -9,
      (0,6,0): 20, 20, 20, 20, 20, 20, 20, -9, -9, -9, -9, -9, -9, -9, -9,
      (0,6,15): -9, -9, -9, -9,
      (0,7,0): 20, 20, 20, 20, 20, 20, 20, -9, -9, -9, -9, -9, -9, -9, -9,
      (0,7,15): -9, -9, -9, -9,
      (0,8,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, 50, 50, 50,
      (0,8,15): 50, 50, 50, 50,
      (0,9,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, 50, 50, 50,
      (0,9,15): 50, 50, 50, 50,
      (0,10,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, 50, 50, 50,
      (0,10,15): 50, 50, 50, 50,
      (0,11,0): -9, -9, -9, -9, 30, 30, 30, 30, 30, 30, 30, -9, -9, -9, -9,
      (0,11,15): -9, -9, -9, -9,
      (0,12,0): -9, -9, -9, -9, 30, 30, 30, 30, 30, 30, 30, -9, -9, -9, -9,
      (0,12,15): -9, -9, -9, -9,
      (1,0,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9,
      (1,0,15): -9, -9, -9, -9,
      (1,1,0): -9, 11, 11, 11, 11, 11, 11, 11, -9, 41, 41, 41, 41, 41, 41,
      (1,1,15): 41, -9, -9, -9,
      (1,2,0): -9, 11, 11, 11, 11, 11, 11, 11, -9, 41, 41, 41, 41, 41, 41,
      (1,2,15): 41, -9, -9, -9,
      (1,3,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, 41, 41, 41, 41, 41, 41,
      (1,3,15): 41, -9, -9, -9,
      (1,4,0): 21, 21, 21, 21, 21, 21, 21, -9, -9, 41, 41, 41, 41, 41, 41,
      (1,4,15): 41, -9, -9, -9,
      (1,5,0): 21, 21, 21, 21, 21, 21, 21, -9, -9, 41, 41, 41, 41, 41, 41,
      (1,5,15): 41, -9, -9, -9,
      (1,6,0): 21, 21, 21, 21, 21, 21, 21, -9, -9, -9, -9, -9, -9, -9, -9,
      (1,6,15): -9, -9, -9, -9,
      (1,7,0): 21, 21, 21, 21, 21, 21, 21, -9, -9, -9, -9, -9, -9, -9, -9,
      (1,7,15): -9, -9, -9, -9,
      (1,8,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, 51, 51, 51,
      (1,8,15): 51, 51, 51, 51,
      (1,9,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, 51, 51, 51,
      (1,9,15): 51, 51, 51, 51,
      (1,10,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, 51, 51, 51,
      (1,10,15): 51, 51, 51, 51,
      (1,11,0): -9, -9, -9, -9, 31, 31, 31, 31, 31, 31, 31, -9, -9, -9, -9,
      (1,11,15): -9, -9, -9, -9,
      (1,12,0): -9, -9, -9, -9, 31, 31, 31, 31, 31, 31, 31, -9, -9, -9, -9,
      (1,12,15): -9, -9, -9, -9,
      (2,0,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9,
      (2,0,15): -9, -9, -9, -9,
      (2,1,0): -9, 12, 12, 12, 12, 12, 12, 12, -9, 42, 42, 42, 42, 42, 42,
      (2,1,15): 42, -9, -9, -9,
      (2,2,0): -9, 12, 12, 12, 12, 12, 12, 12, -9, 42, 42, 42, 42, 42, 42,
      (2,2,15): 42, -9, -9, -9,
      (2,3,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, 42, 42, 42, 42, 42, 42,
      (2,3,15): 42, -9, -9, -9,
      (2,4,0): 22, 22, 22, 22, 22, 22, 22, -9, -9, 42, 42, 42, 42, 42, 42,
      (2,4,15): 42, -9, -9, -9,
      (2,5,0): 22, 22, 22, 22, 22, 22, 22, -9, -9, 42, 42, 42, 42, 42, 42,
      (2,5,15): 42, -9, -9, -9,
      (2,6,0): 22, 22, 22, 22, 22, 22, 22, -9, -9, -9, -9, -9, -9, -9, -9,
      (2,6,15): -9, -9, -9, -9,
      (2,7,0): 22, 22, 22, 22, 22, 22, 22, -9, -9, -9, -9, -9, -9, -9, -9,
      (2,7,15): -9, -9, -9, -9,
      (2,8,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, 52, 52, 52,
      (2,8,15): 52, 52, 52, 52,
      (2,9,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, 52, 52, 52,
      (2,9,15): 52, 52, 52, 52,
      (2,10,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, 52, 52, 52,
      (2,10,15): 52, 52, 52, 52,
      (2,11,0): -9, -9, -9, -9, 32, 32, 32, 32, 32, 32, 32, -9, -9, -9, -9,
      (2,11,15): -9, -9, -9, -9,
      (2,12,0): -9, -9, -9, -9, 32, 32, 32, 32, 32, 32, 32, -9, -9, -9, -9,
      (2,12,15): -9, -9, -9, -9,
      (3,0,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9,
      (3,0,15): -9, -9, -9, -9,
      (3,1,0): -9, 13, 13, 13, 13, 13, 13, 13, -9, 43, 43, 43, 43, 43, 43,
      (3,1,15): 43, -9, -9, -9,
      (3,2,0): -9, 13, 13, 13, 13, 13, 13, 13, -9, 43, 43, 43, 43, 43, 43,
      (3,2,15): 43, -9, -9, -9,
      (3,3,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, 43, 43, 43, 43, 43, 43,
      (3,3,15): 43, -9, -9, -9,
      (3,4,0): 23, 23, 23, 23, 23, 23, 23, -9, -9, 43, 43, 43, 43, 43, 43,
      (3,4,15): 43, -9, -9, -9,
      (3,5,0): 23, 23, 23, 23, 23, 23, 23, -9, -9, 43, 43, 43, 43, 43, 43,
      (3,5,15): 43, -9, -9, -9,
      (3,6,0): 23, 23, 23, 23, 23, 23, 23, -9, -9, -9, -9, -9, -9, -9, -9,
      (3,6,15): -9, -9, -9, -9,
      (3,7,0): 23, 23, 23, 23, 23, 23, 23, -9, -9, -9, -9, -9, -9, -9, -9,
      (3,7,15): -9, -9, -9, -9,
      (3,8,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, 53, 53, 53,
      (3,8,15): 53, 53, 53, 53,
      (3,9,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, 53, 53, 53,
      (3,9,15): 53, 53, 53, 53,
      (3,10,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, 53, 53, 53,
      (3,10,15): 53, 53, 53, 53,
      (3,11,0): -9, -9, -9, -9, 33, 33, 33, 33, 33, 33, 33, -9, -9, -9, -9,
      (3,11,15): -9, -9, -9, -9,
      (3,12,0): -9, -9, -9, -9, 33, 33, 33, 33, 33, 33, 33, -9, -9, -9, -9,
      (3,12,15): -9, -9, -9, -9,
      (4,0,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9,
      (4,0,15): -9, -9, -9, -9,
      (4,1,0): -9, 14, 14, 14, 14, 14, 14, 14, -9, 44, 44, 44, 44, 44, 44,
      (4,1,15): 44, -9, -9, -9,
      (4,2,0): -9, 14, 14, 14, 14, 14, 14, 14, -9, 44, 44, 44, 44, 44, 44,
      (4,2,15): 44, -9, -9, -9,
      (4,3,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, 44, 44, 44, 44, 44, 44,
      (4,3,15): 44, -9, -9, -9,
      (4,4,0): 24, 24, 24, 24, 24, 24, 24, -9, -9, 44, 44, 44, 44, 44, 44,
      (4,4,15): 44, -9, -9, -9,
      (4,5,0): 24, 24, 24, 24, 24, 24, 24, -9, -9, 44, 44, 44, 44, 44, 44,
      (4,5,15): 44, -9, -9, -9,
      (4,6,0): 24, 24, 24, 24, 24, 24, 24, -9, -9, -9, -9, -9, -9, -9, -9,
      (4,6,15): -9, -9, -9, -9,
      (4,7,0): 24, 24, 24, 24, 24, 24, 24, -9, -9, -9, -9, -9, -9, -9, -9,
      (4,7,15): -9, -9, -9, -9,
      (4,8,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, 54, 54, 54,
      (4,8,15): 54, 54, 54, 54,
      (4,9,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, 54, 54, 54,
      (4,9,15): 54, 54, 54, 54,
      (4,10,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, 54, 54, 54,
      (4,10,15): 54, 54, 54, 54,
      (4,11,0): -9, -9, -9, -9, 34, 34, 34, 34, 34, 34, 34, -9, -9, -9, -9,
      (4,11,15): -9, -9, -9, -9,
      (4,12,0): -9, -9, -9, -9, 34, 34, 34, 34, 34, 34, 34, -9, -9, -9, -9,
      (4,12,15): -9, -9, -9, -9,
      (5,0,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9,
      (5,0,15): -9, -9, -9, -9,
      (5,1,0): -9, 15, 15, 15, 15, 15, 15, 15, -9, 45, 45, 45, 45, 45, 45,
      (5,1,15): 45, -9, -9, -9,
      (5,2,0): -9, 15, 15, 15, 15, 15, 15, 15, -9, 45, 45, 45, 45, 45, 45,
      (5,2,15): 45, -9, -9, -9,
      (5,3,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, 45, 45, 45, 45, 45, 45,
      (5,3,15): 45, -9, -9, -9,
      (5,4,0): 25, 25, 25, 25, 25, 25, 25, -9, -9, 45, 45, 45, 45, 45, 45,
      (5,4,15): 45, -9, -9, -9,
      (5,5,0): 25, 25, 25, 25, 25, 25, 25, -9, -9, 45, 45, 45, 45, 45, 45,
      (5,5,15): 45, -9, -9, -9,
      (5,6,0): 25, 25, 25, 25, 25, 25, 25, -9, -9, -9, -9, -9, -9, -9, -9,
      (5,6,15): -9, -9, -9, -9,
      (5,7,0): 25, 25, 25, 25, 25, 25, 25, -9, -9, -9, -9, -9, -9, -9, -9,
      (5,7,15): -9, -9, -9, -9,
      (5,8,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, 55, 55, 55,
      (5,8,15): 55, 55, 55, 55,
      (5,9,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, 55, 55, 55,
      (5,9,15): 55, 55, 55, 55,
      (5,10,0): -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, 55, 55, 55,
      (5,10,15): 55, 55, 55, 55,
      (5,11,0): -9, -9, -9, -9, 35, 35, 35, 35, 35, 35, 35, -9, -9, -9, -9,
      (5,11,15): -9, -9, -9, -9,
      (5,12,0): -9, -9, -9, -9, 35, 35, 35, 35, 35, 35, 35, -9, -9, -9, -9,
      (5,12,15): -9, -9, -9, -9
      }
   }
}
}