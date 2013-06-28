Name:       capi-media-image-util
Summary:    A Image Utility library in Tizen Native API
Version:    0.1.0
Release:    21
Group:      Multimedia/API
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
Source1001: 	capi-media-image-util.manifest
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(mm-common)
BuildRequires:  pkgconfig(mmutil-jpeg)
BuildRequires:  pkgconfig(mmutil-imgp)
BuildRequires:  pkgconfig(capi-base-common)

BuildRequires:  cmake
BuildRequires:  gettext-devel

%description
A Image Utility library in Tizen Native API


%package devel 
Summary:    A Image Utility library in Tizen Native API (Development)
Group:      Multimedia/Development
Requires:   %{name} = %{version}-%{release}
Requires:  pkgconfig(dlog)
Requires:  pkgconfig(mm-common)
Requires:  pkgconfig(mmutil-jpeg)
Requires:  pkgconfig(mmutil-imgp)
Requires:  pkgconfig(capi-base-common)

%description devel
A Image Utility library in Tizen Native API (Development)

%prep
%setup -q
cp %{SOURCE1001} .

%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
%cmake . -DFULLVER=%{version} -DMAJORVER=${MAJORVER}
make %{?jobs:-j%jobs}

%install

%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%manifest %{name}.manifest
%license LICENSE.APLv2.0
%manifest capi-media-image-util.manifest
%{_libdir}/lib*.so.*

%files devel 
%manifest %{name}.manifest
%{_libdir}/lib*.so
%{_libdir}/pkgconfig/*.pc
%{_includedir}/media/*.h

