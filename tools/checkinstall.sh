#!/bin/bash
# $Id: checkinstall.sh 204 2012-01-10 21:37:14Z wus $

[ -d tmp ] && rm -R -f tmp || true
svn export `svn info | awk '/URL/ {print $2}'` tmp
cd tmp

[ -f ../doc/CHANGELOG ] && cp ../doc/CHANGELOG doc/CHANGELOG 
[ -f ../doc/TODO ]      && cp ../doc/TODO      doc/TODO 

make

sudo checkinstall --pkgname "$1" --maintainer "$3" \
  --pkgversion "$2" --pkglicense "GPL 2" -y --pkggroup admin \
  --requires=apt --install=no

sudo checkinstall --pkgname "$1" --maintainer "$3" \
  --pkgversion "$2" --pkglicense "GPL 2" -y --pkggroup admin \
  --requires=apt --arch=i686 --install=no

cp *.deb ../builds/
sudo rm *.deb

cd ..
#rm -Rf tmp