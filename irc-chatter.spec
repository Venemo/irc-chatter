# File: irc-chatter.spec
%define Werror_cflags	%nil

%define patchversion 0

Summary: TODO
Name: irc-chatter
Version: 0.0.0
Release: 0
URL: http://rzr.online.fr/q/meego
Source0: http://rzr.online.fr/ubuntu/pool/main/p/irc-chatter/%{name}_%{version}.orig.tar.gz
Patch0: http://rzr.online.fr/ubuntu/pool/main/p/irc-chatter/%{name}_%{version}-%{patchversion}.diff.gz
License: GPL
Group: TODO
BuildRoot: %{_tmppath}/%{name}-%{version}-root
BuildRequires: qt-qmake
BuildRequires: IrcClient-devel

%description
TODO
  
%prep  
%setup -q -n irc-chatter-%{version}
%patch0 -p1  
  
qmake-qt4 || qmake


%build
make -k

%install
rm -rf ${RPM_BUILD_ROOT}

%makeinstall
make install DESTDIR=$RPM_BUILD_ROOT


%clean
#rm -rf $RPM_BUILD_ROOT
make clean


%files
%defattr(-,root,root)
#%doc docs/*
%{_bindir}/*
%{_libdir}/*
%{_datadir}/*
#%doc docs/*


%changelog
* Fri Jul 01 2011 Philippe Coval <rzr@gna.org> 0.0.0
- http://rzr.online.fr/q/meego
#
