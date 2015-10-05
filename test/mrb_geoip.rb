##
## GeoIP Test
##

GeoIPdat = "/usr/share/GeoIP/GeoIPCity.dat"
GeoIPhost = "www.google.com"

assert("GeoIP#new") do
  geoip = GeoIP.new GeoIPdat

  assert_not_equal(nil, geoip)
end

assert("GeoIP#country_code") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_name GeoIPhost

  assert_equal("US", geoip.country_code)
end

assert("GeoIP#region") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_name GeoIPhost

  assert_equal("CA", geoip.region)
end

assert("GeoIP#region_name") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_name GeoIPhost

  assert_equal("California", geoip.region_name)
end

assert("GeoIP#city") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_name GeoIPhost

  assert_equal("Mountain View", geoip.city)
end

assert("GeoIP#postal_code") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_name GeoIPhost

  assert_equal("94043", geoip.postal_code)
end

assert("GeoIP#latitude") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_name GeoIPhost

  assert_equal(37.4192, geoip.latitude.round(4))
end

assert("GeoIP#longitude") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_name GeoIPhost

  assert_equal(-122.0574, geoip.longitude.round(4))
end

assert("GeoIP#metro_code") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_name GeoIPhost

  assert_equal(807, geoip.metro_code)
end

assert("GeoIP#area_code") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_name GeoIPhost

  assert_equal(650, geoip.area_code)
end

assert("GeoIP#time_zone") do
  geoip = GeoIP.new GeoIPdat
  geoip.record_by_name GeoIPhost

  assert_equal("America/Los_Angeles", geoip.time_zone)
end

assert("GeoIP#close") do
  geoip = GeoIP.new GeoIPdat

  assert_equal(nil, geoip.close)
end

