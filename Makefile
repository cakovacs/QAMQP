all: Makefile_lib Makefile_app
	$(MAKE) -f Makefile_lib install 
	$(MAKE) -f Makefile_app 

Makefile_lib: qamqp_lib.pro
	qmake -o $@ $<

Makefile_app: qamqp_app.pro
	qmake -o $@ $<

clean:
	rm -rf install .obj Makefile_app Makefile_lib Makefile

.PHONY: Makefile_lib Makefile_app
