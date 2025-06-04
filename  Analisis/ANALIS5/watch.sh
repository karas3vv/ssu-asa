#!/bin/bash

while inotifywait -e close_write *.tex *.bib; do
    pdflatex -shell-escape main.tex
done
