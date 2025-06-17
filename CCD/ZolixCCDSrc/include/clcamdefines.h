/**
 *  \file clcamdefines.h
 *  This file holds defines and constants used by the clcamiface DLL and the underlying
 *  camera interface protocol.
 *
 *  \author  Critical Link LLC
 *
 *  \date    6/16/2009
 *
 *  \verbatim

      o  0
      | /       Copyright (c) 2009
     (CL)---o   Critical Link, LLC
       \
        O

    \endverbatim
 **/

#ifndef CLCAMDEFINES_H
#define CLCAMDEFINES_H


/**
 * \defgroup BinConstants Constants used in configuring CCD binning areas.
 * 
 */
//@{
#define BINCODE_BIN       0  //!< Set this to keep the row in the CCD Readout Register (CCD Binning with the next row)
#define BINCODE_DISCARD   6  //!< Set this to read the row out of the CCD and discard it
#define BINCODE_SUM       4  //!< Set this to read the row out of the CCD and SUM in the current FPGA row
#define BINCODE_SEND      5  //!< Set this to read ou the row, sum in FPGA, send results to PC (a new row is started in the FPGA)
#define CL_MAX_BINSETS    8  //!< The maximum number of bin sets supported by the camera
#define MAX_VERT_BIN_SETS CL_MAX_BINSETS
#define MAX_HOR_BIN_SETS  CL_MAX_BINSETS
#define MAX_NUM_BIN_SEQS  16 //!< THe maximum number of binning sequences
//@}

#define MAX_PIXELS_PER_MSG     4096   //!< Maximum number of pixels in a image data message
#define CCDSP_AREA_READ        0      //!< Area Read message/data
#define CCDSP_BINNED_READ      1      //!< Binned Read message/data
#define CCDSP_MASKED_READ_VERT 2      //!< V masked Read message/data
#define CCDSP_MASKED_READ_HORI 3      //!< H masked Read message/data
#define CCDSP_CUSTOM_READ      4      //!< Custom Read message/data
#define CCDSP_AREA8_READ       5      //!< 8-Bit Pixel Area Read message/data
#define MAX_GAIN_SETTINGS      4      //!< maximum gain settings supported in camera

#define MAX_NUM_MULTIPLE_ROI   16     //!< maximum number of multiple windows a multiple-ROI camera can have (used for the size of the message).

#define CCDSP_MAX_ERROR_LENGTH 256    //!< Maximum length of an error message

#define SIMTYPE_NONE         0  //!< use real CCD data
#define SIMTYPE_DC           1  //!< each read pixel from camera is replaced with DC value in mnSimParams[0]
#define SIMTYPE_RAMP         2  //!< pixels are incremented by one starting at zero
#define SIMTYPE_GRADIENT_X   3  //!< pixels are scaled from mnSimParams[0] to mnSimParams[1] by mnSimParams[2] in X dimension
#define SIMTYPE_GRADIENT_Y   4  //!< pixels are scaled from mnSimParams[0] to mnSimParams[1] by mnSimParams[2] in X dimension
#define SIMTYPE_CROSSHAIR    5  //!< two values, cross hairs DC value in mnSimParams[0], value 2 in mnSimParams[1], every [2] in X, [3] in Y
#define SIMTYPE_RANDOM       6  //!< gaussian, mean is mnSimParams[0], std is 0.1 * mnSimParams[1]

#define TRIGMODEONEPERIMAGE  0  //!< one trigger required per image
#define TRIGMODEONEPERGROUP  1  //!< one trigger required per group

#endif // CLCAMDEFINES_H
