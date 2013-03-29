# $Id: Makefile 205 2012-01-10 21:43:28Z wus $

# package
maintainer = Simon Wunderlin
output = pk
version = 1.1.12

# debug default settings, should be 0
dbg = 0

# install
prefix = /usr/local/bin
owner = root
group = root
mode = 0755

# compile
files = upkg.cpp stringlib.cpp options.cpp
object = upkg.o stringlib.o options.o
opitimize = -O2
warnings = -Wall
options = -DUPKG_OUTPUT=$(output) -DUPKG_VERSION="$(version)" -DUPKG_DEBUG=$(dbg) -DUPKG_MAINTAINER="$(maintainer)"

# generate changelog command
changelog = [ -f doc-pak/CHANGELOG ] && rm -f doc-pak/CHANGELOG || true; \
	[ -x ./tools/svn2cl-0.13/svn2cl.sh ] && ./tools/changelog.sh

# generate todo list command
todo = [ -f doc-pak/TODO ] && rm -f doc-pak/TODO || true; ./tools/todo.sh

links = s l i h u p r c

# normal binary
all: dynamic 
	for f in $(links); do \
		[ ! -L $(output)$$f ] && ln -s $(output) $(output)$$f || true; \
	done

static:
	g++ $(options) $(warnings) -s $(opitimize) -c $(files)
	g++ $(options) $(warnings) -static -s $(opitimize) -o $(output) $(object)

dynamic: 
	g++ $(options) $(warnings) -s $(opitimize) -c $(files)
	g++ $(options) $(warnings) -s $(opitimize) -o $(output) $(object)

# development binary	
dev: debug2

popen:
	g++ $(options) $(warnings) -o test/popen test/popen.cpp

osx:
	g++ $(options) $(warnings) -s $(opitimize) -o $(output) $(files)

# compile 386 binary
i686: docu
	# Ubuntu/Debian install: libc6-dev-i386 lib32gcc1 g++-multilib
	g++ $(options) $(warnings) -static -s -m32 $(opitimize) -o $(output) $(files)

# with debug symbols and UPKG_DEBUG=1
debug: clean;
	$(changelog)
	$(todo)
	g++ $(options) $(warnings) $(opitimize) \
		-DUPKG_DEBUG=1 -c $(files)
	g++ $(options) $(warnings) -static $(opitimize) \
		-DUPKG_DEBUG=1 -o $(output) $(object)

# with debug symbols and UPKG_DEBUG=2
debug2: clean;
	$(changelog)
	$(todo)
	g++ $(options) $(warnings) $(opitimize) -DUPKG_DEBUG=2 \
		-c $(files)
	g++ $(options) $(warnings) -static $(opitimize) -DUPKG_DEBUG=2 \
		-o $(output) $(object)

# compile class
class:
	g++ $(options) $(warnings) $(opitimize) -o test/class \
		test/class.cpp stringlib.cpp 

# compile options
options:
	g++ $(options) $(warnings) $(opitimize) -o test/options \
		test/options.cpp options.cpp

# install upkg
install:
	install --group=$(group) --owner=$(owner) --mode=$(mode) $(output) \
		$(prefix)/$(output)
	
	for f in $(links); do \
		[ ! -L $(output)$$f ] && ln -s $(output) $(output)$$f || true; \
	done
	
	for f in $(links); do \
		install --group=$(group) --owner=$(owner) --mode=$(mode) $(output)$$f \
			$(prefix)/$(output)$$f; \
	done

# generate documentation
docu:
	$(changelog)
	$(todo)

# color diff
diff:
	./tools/svndiff.sh

# create deb package
deb:
	#$(changelog)
	#$(todo)
	
	#rm -R -f tmp
	#svn export `svn info | awk '/URL/ {print $$2}'` tmp
	#cd tmp
	#rm -Rf 
	./tools/checkinstall.sh $(output) "$(version)" "$(maintainer)"
	#cd ..

cscope:
	find . -iname '*.cpp' -o -iname '*.h' > cscope.files
	cscope -R -b

# uninstall upkg
uninstall:
	$(output)r $(output) || true
	[ -f $(prefix)/$(output) ] && rm -f $(prefix)/$(output) || true

# remove compiled
clean:
	rm -f doc-pak/TODO       || true
	rm -f doc-pak/CHANGELOG  || true
	rm -f $(output)          || true
	rm -f *.o                || true
	for f in $(links); do \
		[ -L $(output)$$f ] && rm $(output)$$f || true; \
	done
	[ -f `ls backup*gz | tail -n1` ] && mv backup*gz backup/ || true

