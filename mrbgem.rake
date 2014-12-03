MRuby::Gem::Specification.new('mruby-geoip') do |spec|
  spec.license = 'MIT'
  spec.authors = 'MATSUMOTO Ryosuke'
  spec.linker.libraries << 'GeoIP'
  spec.version = '0.0.1'
  spec.summary = 'GeoIP binding for mruby'
end
