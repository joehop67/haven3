CPPFLAGS = -Wall -pipe -mthreads -D__GNUWIN32__ -D__WXMSW__ -DwxUSE_UNICODE -g -D__WXDEBUG__ -IC:\wxwgcc\include -IC:\wxwgcc\lib\gcc810_x64_dll_dev\mswud
LDFLAGS =  -lwxmsw31ud_core -lwxbase31ud -lwxscintillad -lwxmsw31ud_stc -lwxmsw31ud_richtext -mwindows
BIN = bin
OBJ = obj

objects = obj/edit.o \
					obj/main.o \
					obj/prefs.o \
					obj/tabview.o

obj/%.o: src/%.cpp
	@if not exist $(@D) mkdir $(@D)
	g++ $(CPPFLAGS) -c $< -o $@

bin/haven.exe: $(objects)
	@if not exist $(@D) mkdir $(@D)
	g++ -LC:\wxwgcc\lib\gcc810_x64_dll_dev -o $@ $(objects) $(LDFLAGS)

run: bin/haven.exe
	./bin/haven.exe

Debug: run

.PHONY: clean
clean:
	@if exist $(OBJ) rmdir /S /Q obj
	@if exist $(BIN) rmdir /S /Q bin

cleanDebug: clean
