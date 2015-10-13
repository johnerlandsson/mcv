CC			:= g++

LIBNAME		:= mcv
SRCDIR 		:= src
BUILDDIR 	:= build
CFLAGS 		:= -Wall -fpic -std=c++11 -Isrc -ggdb
LIBS 		= -lopencv_core -lopencv_imgproc 
TARGET 		:= lib$(LIBNAME).so
LDFLAGS 	:= -shared -Wl,-soname=$(TARGET) 

SRCEXT 		= cpp
SOURCES 	:= $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS 	:= $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o)) #build/gitversion.o
DEPS 		:= $(OBJECTS:.o=.deps)

all: $(TARGET)

#build/gitversion.o: src/gitversion.cpp
#	$(CC) -c src/gitversion.cpp -o build/gitversion.o $(CFLAGS)

#src/gitversion.cpp: ../.git/HEAD ../.git/index
#	echo "#include <string>" > $@
#	echo "extern const std::string $(LIBNAME)_gitversion = \"$(shell git rev-parse --short HEAD)\";" >> $@

$(TARGET): $(OBJECTS)
	@mkdir -p ../lib/
	@echo " Linking..." 
	$(CC) $(LDFLAGS) $^ -o ../lib/$(TARGET) $(LIBS)
	@mkdir -p ../include/ 2> /dev/null
	@mkdir -p ../include/$(LIBNAME) 2> /dev/null
	-@cp -f src/*.h ../include/$(LIBNAME)/ 
	
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " CC $<"; $(CC) $(CFLAGS) -MD -MF $(@:.o=.deps) -c -o $@ $<
	
clean:
	@echo " Cleaning..."; $(RM) -r $(BUILDDIR) ../lib/$(TARGET)
	$(RM) -f src/gitversion.cpp
	
-include $(DEPS)

.PHONY: clean
