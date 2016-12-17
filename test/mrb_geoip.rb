##
## GeoIP Test
##

GeoIPdat = '/usr/local/share/GeoLiteCity.dat'
GeoIPhost = '216.58.200.196'

# from: https://github.com/maxmind/geoip-api-php/blob/master/tests/data/GeoIPOrg.dat
GeoIPIspdat = '/usr/local/share/GeoIPOrg.dat'

assert("GeoIP#new") do
  geoip = GeoIP.new GeoIPdat

  assert_not_equal(nil, geoip)
end

assert("GeoIP#country_code") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_addr GeoIPhost

  assert_equal("US", geoip.country_code)
end

assert("GeoIP#region") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_addr GeoIPhost

  assert_equal("CA", geoip.region)
end

assert("GeoIP#region_name") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_addr GeoIPhost

  assert_equal("California", geoip.region_name)
end

assert("GeoIP#city") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_addr GeoIPhost

  assert_equal("Mountain View", geoip.city)
end

assert("GeoIP#postal_code") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_addr GeoIPhost

  assert_equal("94043", geoip.postal_code)
end

assert("GeoIP#org") do
  geoip = GeoIP.new(GeoIPdat, GeoIPIspdat)
  geoip.record_by_addr '64.17.254.216'

  assert_equal('Karlin Peebles LLP', geoip.org)
end

assert("GeoIP#latitude") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_addr GeoIPhost

  assert_equal(37.4192, geoip.latitude.round(4))
end

assert("GeoIP#longitude") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_addr GeoIPhost

  assert_equal(-122.0574, geoip.longitude.round(4))
end

assert("GeoIP#metro_code") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_addr GeoIPhost

  assert_equal(807, geoip.metro_code)
end

assert("GeoIP#area_code") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_addr GeoIPhost

  assert_equal(650, geoip.area_code)
end

assert("GeoIP#time_zone") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_addr GeoIPhost

  assert_equal("America/Los_Angeles", geoip.time_zone)
end

assert("GeoIP#close") do
  geoip = GeoIP.new GeoIPdat

  assert_equal(nil, geoip.close)
end
