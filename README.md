# mruby-geoip   [![Build Status](https://travis-ci.org/matsumoto-r/mruby-geoip.png?branch=master)](https://travis-ci.org/matsumoto-r/mruby-geoip)
GeoIP class for mruby
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :git => 'https://github.com/matsumoto-r/mruby-geoip.git'
end
```
## example
```ruby
db_path = "/usr/share/GeoIP/GeoIPCity.dat"
host = "www.google.com"

geoip = GeoIP.new db_path, host

# You can use record_by_addr when using IP address into host
geoip.record_by_name

geoip.country_code #=> US
geoip.region #=> CA
```

## License
under the MIT License:
- see LICENSE file
