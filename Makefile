SOURCE_FOLDER := src
OBJECT_FOLDER := obj
INCLUDE_FOLDER := include
LIBRARY_FOLDER := lib

C := .cpp
O := .o
H := .hpp

GCC := g++ -std=c++20

EXE := main

LINK_FLAGS := -lraylib -lpendulum -lopengl32 -lgdi32 -lwinmm

# PENDULUM LIBRARY #
PENDULUM_FILE := pendulum
PENDULUM_SOURCE := $(SOURCE_FOLDER)/$(PENDULUM_FILE)$(C)
PENDULUM_OBJECT := $(OBJECT_FOLDER)/$(PENDULUM_FILE)$(O)
PENDULUM_LIB := $(LIBRARY_FOLDER)/lib$(PENDULUM_FILE).a
# ---------------- #

SOURCE := $(wildcard $(SOURCE_FOLDER)/*$(C))
SOURCE := $(filter-out $(PENDULUM_SOURCE),$(SOURCE))
OBJECT := $(patsubst $(SOURCE_FOLDER)/%$(C),$(OBJECT_FOLDER)/%$(O),$(SOURCE))
HEADER := $(wildcard $(INCLUDE_FOLDER)/*$(H))

LINK := -L$(LIBRARY_FOLDER) $(LINK_FLAGS)


all: $(OBJECT) $(HEADER)
	$(GCC) $(OBJECT) -o $(EXE) $(LINK)

force: clean all

$(OBJECT_FOLDER)/%$(O): $(SOURCE_FOLDER)/%$(C) $(HEADER) | $(OBJECT_FOLDER)
	$(GCC) -I$(INCLUDE_FOLDER) -c $< -o $@

$(OBJECT_FOLDER):
	@mkdir $(OBJECT_FOLDER)


# Pendulum library #

pendulum: $(PENDULUM_OBJECT) $(HEADER) | $(OBJECT_FOLDER)
	ar rcs $(PENDULUM_LIB) $(PENDULUM_OBJECT)

# PHONY #

.PHONY: clean
clean:
	@if exist $(OBJECT_FOLDER) (rmdir /s /q $(OBJECT_FOLDER))
	@del /Q *.exe 2>NUL
	$(info Removed "obj" folder)
	$(info Removed "exe" files)

.PHONY: info
info:
	$(info $(HEADER))
