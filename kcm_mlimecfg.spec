Summary: Magic Linux IME Control Center Modules
Summary(zh_CN.GB18030): Magic Linux ���뷨���ÿ�������ģ��
Name: kcm_mlimecfg
Version: 1.5.0
Release: 1mgc
Group: User Interface/X
Group(zh_CN.GB18030): �û�����/X
License: GPL v2+
Source: mlimecfg.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

URL: http://www.magiclinux.org
Distribution: Magic Linux

BuildRequires: cmake >= 2.6.0
BuildRequires: qt4-devel
BuildRequires: libkdelibs4-devel
BuildRequires: gettext
Requires: kdebase4-runtime

%description
Magic Linux IME Control Center Modules.

%description -l zh_CN.GB18030
Magic Linux ���뷨���ÿ�������ģ�顣

%prep
%setup -q -n mlimecfg

%build
mkdir build
cd build
%cmake_kde4 ..

make

%install
rm -rf %{buildroot}
cd build
make DESTDIR=%{buildroot} install

# ��װ���뷨���ã���װ·��Ӳ�����ڳ����У����ɸ���
mkdir -p %{buildroot}%{_sysconfdir}/ime
install -m 664 src/ime/fcitx %{buildroot}%{_sysconfdir}/ime/fcitx
install -m 664 src/ime/scim %{buildroot}%{_sysconfdir}/ime/scim

%clean
rm -rf %{buildroot} %{_builddir}/%{buildsubdir}

%files
%defattr(-,root,root,-)
%doc AUTHORS ChangeLog COPYING README TODO
%{_sysconfdir}/ime/*
%{kde4_plugindir}/kcm_mlimecfg.so
%{kde4_servicesdir}/mlimecfg.desktop
%{kde4_localedir}/zh_CN/LC_MESSAGES/kcm_mlimecfg.mo

%changelog
* Wed Jul 29 2009 Ni Hui <shuizhuyuanluo@126.com> - 1.5.0-1mgc
- ��ֲ�� cmake/Qt4/KDE4
- ���ʻ�֧��
- ����  ���³���
