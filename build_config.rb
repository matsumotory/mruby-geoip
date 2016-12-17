MRuby::Build.new do |conf|
  toolchain :gcc
  conf.gembox 'default'
  conf.gem File.expand_path(File.dirname(__FILE__))
  conf.gem :github => 'iij/mruby-io'
  conf.enable_test
end
