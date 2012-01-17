Summary: A plug-in for Gimp to load/save IFF-ILBM images
Name: gimp-ilbm
Version: 0.9.4
Release: 1
Copyright: GPL
Group: Applications/Multimedia
Source: http://www.dummy.de/gimpilbm/gimp-ilbm-0.9.4.tar.gz
URL: http://www.dummy.de/gimpilbm/
BuildRoot: /tmp/%{name}-buildroot
Packager: Johannes Teveﬂen <j.tevessen@gmx.net>
Requires: gimp >= 1.2.0
Prefix: /usr

%description
A plugin for The GIMP that allows to read and write
IFF-ILBM files as commonly used on Amiga systems.
Supports several formats including HAM, EHB and 24bit.

Compression also supported as well as transparency.

%prep
%setup -q

%build
./configure --with-gimp=$RPM_BUILD_ROOT%{prefix}/lib/gimp/1.2 --with-debug=debug
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi
make install

%clean
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi

%files
%defattr(-,root,root)
%doc AUTHORS ChangeLog COPYING INSTALL NEWS README

%{prefix}/lib/gimp/1.2/plug-ins/ilbm

