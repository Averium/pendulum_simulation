SOURCE_FOLDER := src
OBJECT_FOLDER := obj
INCLUDE_FOLDER := include
LIBRARY_FOLDER := lib

C := .cpp
O := .o
H := .hpp

GCC := g++ -std=c++20

EXE := main

RAYLIB_LINK_FLAGS := -lraylib -lopengl32 -lgdi32 -lwinmm

SOURCE := $(wildcard $(SOURCE_FOLDER)/*$(C))
OBJECT := $(patsubst $(SOURCE_FOLDER)/%$(C),$(OBJECT_FOLDER)/%$(O),$(SOURCE))
HEADER := $(wildcard $(INCLUDE_FOLDER)/*$(H))

LINK := -L$(LIBRARY_FOLDER) $(RAYLIB_LINK_FLAGS)


all: $(OBJECT) $(HEADER)
	$(GCC) $(OBJECT) -o $(EXE) $(LINK)

force: clean all

$(OBJECT_FOLDER)/%$(O): $(SOURCE_FOLDER)/%$(C) $(HEADER) | $(OBJECT_FOLDER)
	$(GCC) -I$(INCLUDE_FOLDER) -c $< -o $@

$(OBJECT_FOLDER):
	@mkdir $(OBJECT_FOLDER)

.PHONY: clean
clean:
	@if exist $(OBJECT_FOLDER) (rmdir /s /q $(OBJECT_FOLDER))
	@del /Q *.exe 2>NUL
	$(info Removed "obj" folder)
	$(info Removed "exe" files)

.PHONY: info
info:
	$(info $(HEADER))
