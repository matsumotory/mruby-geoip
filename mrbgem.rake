MRuby::Gem::Specification.new('mruby-geoip') do |spec|
  spec.license = 'MIT'
  spec.authors = 'MATSUMOTO Ryosuke'
  spec.linker.libraries << 'GeoIP'
  spec.version = '0.0.1'
  spec.summary = 'GeoIP binding for mruby'

  task :test => "#{build.build_dir}/../GeoLiteCity.dat"
  file "#{build.build_dir}/../GeoLiteCity.dat" do |t|
    sh "curl -L http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz | gunzip > '#{build.build_dir}/../GeoLiteCity.dat'"
  end

  task :test => "#{build.build_dir}/../GeoLiteIPOrg.dat"
  file "#{build.build_dir}/../GeoLiteIPOrg.dat" do |t|
    sh "curl -L https://github.com/maxmind/geoip-api-php/raw/master/tests/data/GeoIPOrg.dat > '#{build.build_dir}/../GeoLiteIPOrg.dat'"
  end
end
