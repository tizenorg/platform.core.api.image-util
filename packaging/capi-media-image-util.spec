Name:       capi-media-image-util
Summary:    A Image Utility library in Tizen Native API
Version:    0.1.0
Release:    9
Group:      TO_BE/FILLED_IN
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
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
Summary:    A Image Utility library in Tizen Native API (Developement)
Group:      TO_BE_FILLED 
Requires:   %{name} = %{version}-%{release}
Requires:  pkgconfig(dlog)
Requires:  pkgconfig(mm-common)
Requires:  pkgconfig(mmutil-jpeg)
Requires:  pkgconfig(mmutil-imgp)
Requires:  pkgconfig(capi-base-common)

%description devel
A Image Utility library in Tizen Native API (Developement)

%prep
%setup -q

%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DFULLVER=%{version} -DMAJORVER=${MAJORVER}
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%post

%postun


%files
%{_libdir}/lib*.so.*

%files devel 
%{_libdir}/lib*.so
%{_libdir}/pkgconfig/*.pc
%{_includedir}/media/*.h

