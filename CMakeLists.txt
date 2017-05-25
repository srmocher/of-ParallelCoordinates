# ======================= ofxCMake =======================
#  PUT THIS FILE INTO YOUR OPENFRAMEWORKS PROJECT FOLDER

# ========================================================
# ===================== CMake Settings ===================
# ========================================================
cmake_minimum_required( VERSION 3.3 )
project( openframeworks )


# ========================================================
# ===================== User Settings ====================
# ========================================================
# ---------------------- App name  -----------------------
set( APP_NAME   ofParallelCoordinates )

# ------------------------ OF Path -----------------------
# --- If outside the OF structure, set an absolute OF path

set( OF_DIRECTORY_BY_USER "../../.." )



# --------------------- Source Files ---------------------
set( ${APP_NAME}_SOURCE_FILES
        src/main.cpp
        src/ofApp.cpp
	src/tablereader.cpp
	src/cluster.cpp
	src/parallelplot.cpp
	src/color.cpp 
	src/column.cpp)


# ------------------------ AddOns  -----------------------
set( OFX_ADDONS_ACTIVE
        #        ofxXmlSettings
        #        ofxOsc
        #        ofxGui
        #        ofxNetwork
        #        ofxVectorGraphics
        #        ofxThreadedImageLoader
        #        ofxAssimpModelLoader
        #        ofxKinect
        #        ofxOpenCv
        #        ofxSvg
        #        ofxEmscripten (untested)
                )

# =========================================================================
# ============================== OpenFrameworks ===========================
# =========================================================================
include( ${OF_DIRECTORY_BY_USER}/addons/ofxCMake/modules/main.cmake )
# =========================================================================

