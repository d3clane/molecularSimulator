CC := g++

CFLAGS = -D _DEBUG -ggdb3 -std=c++17 -O3 -Wall -Wextra -Weffc++ \
		   -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts 		  \
		   -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal      \
		   -Wformat-nonliteral -Wformat-security -Wformat=2 \
		   -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self \
		   -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo		  \
		   -Wstrict-overflow=2 \
		   -Wsuggest-override -Wswitch-default -Wswitch-enum 		  \
		   -Wundef -Wunreachable-code -Wunused -Wvariadic-macros   \
		   -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs 			  \
		   -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow 	  \
		   -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-protector  						  \
		   -fPIE -Werror=vla									  

OUT_O_DIR := build
COMMONINC := -I./include -I./
LIB_INC   := -isystem/opt/homebrew/Cellar/sfml/2.6.1/include
LIB_LINK  := -L/opt/homebrew/Cellar/sfml/2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system

LDFLAGS   := $(LIB_LINK)

PROGRAM_DIR  := $(OUT_O_DIR)/bin
PROGRAM_NAME := molecules.out

TESTS = ./Tests

ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

override CFLAGS += $(COMMONINC)
override CFLAGS += $(LIB_INC)

CPPSRC = src/Graphics/Events.cpp src/Graphics/GraphicsWindow.cpp src/Graphics/Mouse.cpp \
		 src/Graphics/PixelsArray.cpp src/Graphics/PrivateConverters.cpp src/Graphics/Sprite.cpp  \
		 src/Graphics/Texture.cpp src/Model/Collider.cpp src/Engine/CoordsSystem.cpp \
		 src/Engine/Transformable.cpp src/Engine/Vector.cpp src/Model/Molecule.cpp \
		 src/Model/MoleculeManager.cpp src/main.cpp src/Model/2DVtable.cpp src/Model/Chemistry.cpp \
		 src/Model/Physics.cpp src/Logger/Log.cpp src/Gui/Button.cpp src/Gui/HoverAnimatedButton.cpp \
		 src/Gui/WindowManager.cpp src/Utils/Rand.cpp src/View/View.cpp src/View/Gui/ButtonActions.cpp \
		 src/Controller/Controller.cpp src/Graphics/Circle.cpp src/View/Gui/GraphsWindow.cpp \
		 src/Scene/Vector.cpp src/Utils/Doubles.cpp src/Graphics/Font.cpp src/Graphics/Text.cpp \
		 src/Graphics/Font.cpp src/Graphics/Text.cpp src/Utils/Exceptions.cpp src/Model/Boundary.cpp

CPPOBJ := $(addprefix $(OUT_O_DIR)/,$(CPPSRC:.cpp=.o))
DEPS = $(CPPOBJ:.o=.d)

.PHONY: all
all: $(PROGRAM_DIR)/$(PROGRAM_NAME)

$(PROGRAM_DIR)/$(PROGRAM_NAME): $(CPPOBJ)
	@mkdir -p $(@D)
	$(CC) $^ -o $@ $(LDFLAGS)

$(CPPOBJ) : $(OUT_O_DIR)/%.o : %.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(DEPS) : $(OUT_O_DIR)/%.d : %.cpp
	@mkdir -p $(@D)
	$(CC) -E $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@

#
#TESTFILES=$(wildcard $(TESTS)/*.dat)
#
#.PHONY: testrun
#testrun: $(TESTFILES)

#.PHONY: $(TESTFILES)
#$(TESTFILES): $(OUT_O_DIR)/LC.x
#	@$(ROOT_DIR)/runtest.sh $@ $(OUT_O_DIR)/LC.x

.PHONY: clean
clean:
	rm -rf $(CPPOBJ) $(DEPS) $(OUT_O_DIR)/*.x $(OUT_O_DIR)/*.log

NODEPS = clean

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
include $(DEPS)
endif