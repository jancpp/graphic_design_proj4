CPP = g++ -std=c++11
INC = -I../cryphutil -I../fontutil -I../glslutil -I../imageutil -I../mvcutil
C_FLAGS = -fPIC -g -c -DGL_GLEXT_PROTOTYPES $(INC)

LINK = g++ -fPIC -g
LOCAL_UTIL_LIBRARIES = ../lib/libcryph.so ../lib/libfont.so ../lib/libglsl.so ../lib/libimage.so ../lib/libmvc.so
GL_LIB_LOC = -L/usr/lib/nvidia-375
GL_LIBRARIES = $(GL_LIB_LOC) -lglfw -lGLU -lGL

OBJS = main.o ExtendedController.o PhongMaterial.o SceneElement.o Table.o Couch.o Room.o Lamp.o Light.o ECenter.o TVSet.o Tree.o Ornaments.o

main: $(OBJS) $(LOCAL_UTIL_LIBRARIES)
	$(LINK) -o main $(OBJS) $(LOCAL_UTIL_LIBRARIES) $(GL_LIBRARIES)

../lib/libcryph.so: ../cryphutil/AffVector.h ../cryphutil/AffVector.c++ ../cryphutil/AffPoint.h ../cryphutil/AffPoint.c++
	(cd ../cryphutil; make)

../lib/libfont.so: ../fontutil/CFont.h ../fontutil/CFont.c++ ../fontutil/CGLString.h ../fontutil/CGLString.c++
	(cd ../fontutil; make)

../lib/libglsl.so: ../glslutil/ShaderIF.h ../glslutil/ShaderIF.c++
	(cd ../glslutil; make)

../lib/libimage.so: ../imageutil/ImageReader.h ../imageutil/ImageReader.c++
	(cd ../imageutil; make)

../lib/libmvc.so: ../mvcutil/Controller.h ../mvcutil/Controller.c++ ../mvcutil/ModelView.h ../mvcutil/ModelView.c++
	(cd ../mvcutil; make)

main.o: main.c++
	$(CPP) $(C_FLAGS) main.c++
ExtendedController.o: ExtendedController.c++
	$(CPP) $(C_FLAGS) ExtendedController.c++
PhongMaterial.o: PhongMaterial.h PhongMaterial.c++
	$(CPP) $(C_FLAGS) PhongMaterial.c++
SceneElement.o: SceneElement.h SceneElement.c++
	$(CPP) $(C_FLAGS) SceneElement.c++
Table.o: Table.h Table.c++
	$(CPP) $(C_FLAGS) Table.c++
Couch.o: Couch.h Couch.c++
	$(CPP) $(C_FLAGS) Couch.c++
Room.o: Room.h Room.c++
	$(CPP) $(C_FLAGS) Room.c++
Lamp.o: Lamp.h Lamp.c++
	$(CPP) $(C_FLAGS) Lamp.c++
Light.o: Light.h Light.c++
	$(CPP) $(C_FLAGS) Light.c++
TVSet.o: TVSet.h TVSet.c++
	$(CPP) $(C_FLAGS) TVSet.c++
ECenter.o: ECenter.h ECenter.c++
	$(CPP) $(C_FLAGS) ECenter.c++
Tree.o: Tree.h Tree.c++
	$(CPP) $(C_FLAGS) Tree.c++
Ornaments.o: Ornaments.h Ornaments.c++
	$(CPP) $(C_FLAGS) Ornaments.c++
