# Contributor: Mark Hessling <mark@rexx.org>
# Maintainer: Mark Hessling <mark@rexx.org>
pkgname="regina-rexx"
pkgver="3.9.3"
pkgrel=1
pkgdesc="Regina Rexx interpreter"
url="http://www.rexx.org"
arch="all"
license="GPL-2.0-or-later"
depends=""
makedepends=""
#install="$pkgname.pre-install $pkgname.post-install"
subpackages="$pkgname-dev $pkgname-doc"
options="!check"
source="
        regina-rexx-3.9.3.tar.gz
        "
builddir="$srcdir/"

build() {
        cd "$srcdir/$pkgname-$pkgver"
        ./configure \
           --build=$CBUILD \
           --host=$CHOST \
           --prefix=/usr \
           --sysconfdir=/etc \
           --mandir=/usr/share/man \
           --localstatedir=/var
        make -j1
}

package() {
        cd "$srcdir/$pkgname-$pkgver"
        make -j1 DESTDIR="$pkgdir" install

#        install -m755 -D "$srcdir"/$pkgname.initd "$pkgdir"/etc/init.d/$pkgname
#        install -m644 -D "$srcdir"/$pkgname.confd "$pkgdir"/etc/conf.d/$pkgname
}
