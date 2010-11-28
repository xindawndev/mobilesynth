repositories.remote << 'http://www.ibiblio.org/maven2'

GUAVA = 'com.google.guava:guava:jar:r07'
EASYMOCK = 'org.easymock:easymock:jar:3.0'
CGLIB = 'cglib:cglib-nodep:jar:2.2'

define 'synth' do
  project.version = '0.0.1'
  project.group = 'thebends'
  compile.with GUAVA
  compile.options.lint = 'all'
  compile.options.other = '-Werror'
  test.with EASYMOCK, CGLIB
  package :jar

  run.using :main => 'org.thebends.synth.PlaySine'
end

