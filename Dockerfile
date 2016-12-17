FROM centos:latest
RUN yum -y groupinstall "Development Tools"
RUN yum -y install ruby \
    ruby-gems \
    yum \
    wget \
    git

RUN gem install rake
RUN git clone https://github.com/maxmind/geoip-api-c
RUN cd geoip-api-c \
    && ./bootstrap \
    && ./configure -prefix=/usr \
    && make \
    && make install
RUN wget -P /usr/local/share/ https://raw.githubusercontent.com/maxmind/geoip-api-php/master/tests/data/GeoIPOrg.dat
RUN wget -P /usr/local/share/ http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz \
    && gunzip /usr/local/share/GeoLiteCity.dat.gz
ENV LD_LIBRARY_PATH /usr/lib
ADD . /tmp/mruby-geoip
WORKDIR /tmp/mruby-geoip
CMD cp .travis_build_config.rb build_config.rb && rake test
