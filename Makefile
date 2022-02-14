#================================[[ beg-code ]]================================#



#===(user defined)=========================================#
# basename of executable, header, and all code files
NAME_BASE  = metis
# additional standard and outside libraries
LIB_STD    = -lX11 -lGL -lGLU -lm -lXext
# all heatherly libraries used, debug versions whenever available
LIB_MINE   = -lX11 -lXext -lGL -lyX11_debug -lyFONT_debug -lySTR_debug -lyGLTEX_debug -lyVIOPENGL_debug -lyMODE_debug -lyKEYS_debug -lyMACRO_debug -lyCMD_debug -lySRC_debug -lyVIEW_debug -lyFILE_debug -lyMAP_debug -lyJOBS_debug -lyREGEX_debug
# directory for production code, no trailing slash
INST_DIR   = /usr/local/bin



#===(optional)=============================================#
# extra include directories required
INC_MINE   = 
# utilities generated, separate from main program
NAME_UTIL  = 
# libraries only for the utilities
LIB_UTIL   = 



#===(post-processing)======================================#
# create a rule for...
#install_prep       :
#remove_prep        :



#===(master template)======================================#
include /usr/local/sbin/make_program



#===(post-processing)======================================#
# create a rule for...
#install_post       :
#remove_post        :



#================================[[ end-code ]]================================#

