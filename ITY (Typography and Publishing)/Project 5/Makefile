
PROJ=proj5
PACK=xjuhas04-fit
IMAGES_DIRECTORY=./img

.PHONY: pdflatex pack 


pdflatex: $(PROJ).tex
	pdflatex --shell-escape $^
	pdflatex --shell-escape $^


pack: clean $(PACK).tar.gz


$(PACK).tar.gz: $(PROJ).tex $(IMAGES_DIRECTORY)
	tar -czf $@ $^ Makefile
