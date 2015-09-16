SRC := Main.c Backdrop.c Utils.c
RC := BackdropResources.rc
OBJ := $(patsubst %.c,%.o,$(SRC)) $(patsubst %.rc,%.o,$(RC))
EXE := Backdrop.exe
OPT := -Wall -pedantic -O2 -D_WIN32_WINNT=0x0501
LIB := -mwindows

.PHONY: all
all: $(EXE)

$(EXE): $(OBJ)
	gcc $(OPT) -o $@ $(OBJ) $(LIB)

-include $(SRC:.c=.d)

%.d: %.c
	gcc -MM -MG $< > $@

%.o: %.c
	gcc $(OPT) -c $< -o $@

%.o: %.rc
	windres -i $< -o $@

.PHONY: clean
clean:
	del $(EXE)
	del *.o
	del *.d
