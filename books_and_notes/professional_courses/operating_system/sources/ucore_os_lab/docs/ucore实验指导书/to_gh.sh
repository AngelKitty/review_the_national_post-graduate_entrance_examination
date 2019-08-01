#!/bin/bash

gitbook build .
cp -rf _book/* ~/tmp/ucore_docs
cd ~/tmp/ucore_docs
git add .
git commit -m 'pub'
git push origin gh-pages
