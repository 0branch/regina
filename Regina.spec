%define debug_package %{nil}
%define vermajor 3
%define verminor 7
Name: Regina-REXX
Version: 3.7
Release: 1
Group: Programming
Source: Regina-REXX-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-root
Prefix: /usr
License: LGPL
Vendor: Mark Hessling
Packager: Mark Hessling
URL: http://regina-rexx.sourceforge.net
Summary: Regina Rexx Interpreter binaries, language files and sample programs.
%ifarch x86_64 ia64 ppc64 s390x sparc64
Provides: %{name}(64bit)
Requires: %{name}-lib(64bit)
%else
Provides: %{name}
Requires: %{name}-lib
%endif
Icon: regina64.xpm
# if we don't have _extension define, define it
# this is because Mandriva defines _extension
%{!?_extension: %define _extension .gz}

%description
Regina is an implementation of a Rexx interpreter, compliant with
the ANSI Standard for Rexx (1996).  It is also available on several other
operating systems.

For more information on Regina, visit http://regina-rexx.sourceforge.net/
For more information on Rexx, visit http://www.rexxla.org

%package devel
%ifarch x86_64 ia64 ppc64 s390x sparc64
Provides: %{name}-dev(64bit) libregina.so()(64bit)
Requires: %{name}-lib(64bit)
%else
Provides: %{name}-dev libregina.so()
Requires: %{name}-lib
%endif
Group: Programming
Summary: Regina Rexx development libraries and header file.
%description devel
Regina is an implementation of a Rexx interpreter, compliant with
the ANSI Standard for Rexx (1996).  It is also available on several other
operating systems.

For more information on Regina, visit http://regina-rexx.sourceforge.net/
For more information on Rexx, visit http://www.rexxla.org

%package lib
%ifarch x86_64 ia64 ppc64 s390x sparc64
Provides: %{name}-lib(64bit)
%else
Provides: %{name}-lib
%endif
Group: Programming
Summary: Regina Rexx runtime libraries.
%description lib
Regina is an implementation of a Rexx interpreter, compliant with
the ANSI Standard for Rexx (1996).  It is also available on several other
operating systems.

For more information on Regina, visit http://regina-rexx.sourceforge.net/
For more information on Rexx, visit http://www.rexxla.org

%prep
%setup

%build
./configure --prefix=%{prefix} --mandir=%{prefix}/share/man
make

%install
rm -fr %{buildroot}
make DESTDIR=%{buildroot} install

%files
/usr/share/man/man1/regina.1%{_extension}
/usr/share/man/man1/rxstack.1%{_extension}
/usr/share/man/man1/rxqueue.1%{_extension}
/usr/etc/rc.d/init.d/rxstack
/usr/share/regina/rexxcps.rexx
/usr/share/regina/animal.rexx
/usr/share/regina/block.rexx
/usr/share/regina/dateconv.rexx
/usr/share/regina/timeconv.rexx
/usr/share/regina/newstr.rexx
/usr/share/regina/dynfunc.rexx
/usr/share/regina/regutil.rexx
/usr/share/regina/de.mtb
/usr/share/regina/en.mtb
/usr/share/regina/es.mtb
/usr/share/regina/no.mtb
/usr/share/regina/pl.mtb
/usr/share/regina/pt.mtb
/usr/share/regina/sv.mtb
/usr/share/regina/tr.mtb
/usr/bin/rexx
/usr/bin/regina
/usr/bin/rxqueue
/usr/bin/rxstack

%files devel
/usr/%{_lib}/libregina.a
/usr/%{_lib}/libregina.so
/usr/include/rexxsaa.h
/usr/share/man/man1/regina-config.1%{_extension}
/usr/bin/regina-config

%files lib
/usr/%{_lib}/libregina.so.%{version}
/usr/%{_lib}/libregina.so.%{vermajor}
/usr/%{_lib}/libregutil.so
/usr/%{_lib}/librxtest1.so
/usr/%{_lib}/librxtest2.so

%post
cd %{prefix}/%{_lib}
ldconfig %{prefix}/%{_lib}
