# mruby-geoip   [![Build Status](https://travis-ci.org/matsumotory/mruby-geoip.png?branch=master)](https://travis-ci.org/matsumotory/mruby-geoip)
GeoIPCity class using [GeoIPCity.dat](http://dev.maxmind.com/geoip/legacy/install/city/) for mruby
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'matsumoto-r/mruby-geoip'
end
```
## example
```ruby
db_path = "/usr/share/GeoIP/GeoIPCity.dat"
host = "www.google.com"

geoip = GeoIP.new db_path

# You can use record_by_addr when using IP address into host
geoip.record_by_name host

geoip.country_code        #=> "US"
geoip.region              #=> "CA"
geoip.region_name         #=> "California"
geoip.city                #=> "Mountain View"
geoip.postal_code         #=> "94043"
geoip.latitude.round(4)   #=> 37.4192
geoip.longitude.round(4)  #=> -122.0574
geoip.metro_code          #=> 807
geoip.area_code           #=> 650
geoip.time_zone           #=> "America/Los_Angeles"
```

## License
under the MIT License:
- see LICENSE file
