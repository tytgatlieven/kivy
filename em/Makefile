#
# This build process uses generated files that undergo
# further processing. The files in gen0 are the targets for compiling,
# i.e. they have been fully processed.
#
PYVERSION=3.5.2
PYMINOR=$(basename $(PYVERSION))

CC=emcc
OPTFLAGS=-O2
CPY_EM=../cpython-emscripten
CPY_EM_INSTALL=$(CPY_EM)/installs/python-$(PYVERSION)
CFLAGS=-std=gnu99 $(OPTFLAGS) \
	-I $(CPY_EM_INSTALL)/include/python$(PYMINOR)/ \
	-I ../kivy/graphics \
	-Wno-warn-absolute-paths \
	-s USE_SDL=2 \
	-s USE_SDL_IMAGE=2 \
	-s SDL2_IMAGE_FORMATS='["png", "jpeg"]' \
	-s USE_SDL_TTF=2

LDFLAGS=$(OPTFLAGS) \
	$(CPY_EM_INSTALL)/lib/libpython$(PYMINOR).a \
	-s TOTAL_MEMORY=268435456 \
	-s ASSERTIONS=2 \
	-s EMULATE_FUNCTION_POINTER_CASTS=1 \
	-s USE_SDL=2 \
	-s USE_SDL_IMAGE=2 \
	-s SDL2_IMAGE_FORMATS='["png", "jpeg"]' \
	-s USE_SDL_TTF=2 \
	-s EXPORTED_FUNCTIONS='["_main","_PyRun_SimpleString"]' \
	--memory-init-file 0

PYX_SRC = js.pyx \
	../kivy/_clock.pyx \
	../kivy/core/image/_img_sdl2.pyx \
	../kivy/core/text/text_layout.pyx \
	../kivy/core/text/_text_sdl2.pyx \
	../kivy/core/window/_window_sdl2.pyx \
	../kivy/_event.pyx \
	../kivy/graphics/buffer.pyx \
	../kivy/graphics/compiler.pyx \
	../kivy/graphics/context_instructions.pyx \
	../kivy/graphics/context.pyx \
	../kivy/graphics/fbo.pyx \
	../kivy/graphics/gl_instructions.pyx \
	../kivy/graphics/instructions.pyx \
	../kivy/graphics/opengl.pyx \
	../kivy/graphics/opengl_utils.pyx \
	../kivy/graphics/scissor_instructions.pyx \
	../kivy/graphics/shader.pyx \
	../kivy/graphics/stencil_instructions.pyx \
	../kivy/graphics/svg.pyx \
	../kivy/graphics/texture.pyx \
	../kivy/graphics/transformation.pyx \
	../kivy/graphics/vbo.pyx \
	../kivy/graphics/vertex_instructions.pyx \
	../kivy/graphics/vertex.pyx \
	../kivy/properties.pyx \
	../kivy/weakproxy.pyx
MOD_SRC=$(patsubst %.pyx,gen0/%.c,$(patsubst ../%,%,$(PYX_SRC)))
MOD_OBJ=$(patsubst gen0/%.c,obj/%.bc,$(MOD_SRC))

KIVY_ZIP_FILES= \
	$(shell find ../kivy -name *.py) \
	$(shell find ../kivy/data -type f)

APP_ZIP_FILES= \
	$(shell find app ! -type d -a ! -name ".*")


all: web/kivy.js web/kivy.zip web/app.zip


clean:
	-rm -fr gen? obj root
	-rm web/kivy.js web/kivy.data web/kivy.zip web/app.zip


web/kivy.js: obj/main.bc $(MOD_OBJ) root $(shell find root)
	$(CC) -o $@ obj/main.bc $(MOD_OBJ) $(LDFLAGS) \
		$(foreach d,$(wildcard root/*),--preload-file $d@/$(notdir $d))


web/kivy.zip: $(KIVY_ZIP_FILES)
	-@rm $@
	( cd .. ; zip $(abspath $@) $(patsubst ../%,%,$^) )


web/app.zip: $(APP_ZIP_FILES)
	-@rm $@
	zip $@ $^


obj/main.bc: main.c gen0/builtins.h
	@mkdir -p $(dir $@)
	$(CC) -o $@ $< $(CFLAGS)


obj/%.bc: gen0/%.c
	@mkdir -p $(dir $@)
	@cp config.h ../kivy/graphics/
	$(CC) -o $@ $< $(CFLAGS)


gen0/%.c: gen1/%.c
	python $(CPY_EM)/utils/patch_cython_module.py $< $@ --start-depth 1
	touch $@


gen1/%.c: gen2/%.c
	@mkdir -p $(dir $@)
	sed -e 's#.*_gilstate.*#//\0#' < $< > $@


gen2/%.c: %.pyx
	@mkdir -p $(dir $@)
	@cp config.pxi ../kivy/graphics/
	cython -o $@ $<


gen2/%.c: ../%.pyx
	@mkdir -p $(dir $@)
	@cp config.pxi ../kivy/graphics/
	cython -o $@ $<


gen0/builtins.h: $(PYX_SRC)
	@mkdir -p $(dir $@)
	python $(CPY_EM)/utils/make_cython_builtins.py $(MOD_SRC) \
		--start-depth 1 > $@


root: lib_files $(wildcard fake_libs/*.py)
	mkdir -p root/lib
	tar -C $(CPY_EM_INSTALL) -cf - --files-from lib_files | tar -C root -xvf -
	cp fake_libs/*.py root/lib/python3.5/
	touch root


.PRECIOUS: gen0/%.c gen1/%.c gen2/%.c
