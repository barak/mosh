(load "./bootstrap.gauche/compat-gauche.scm")
(load "./runtime.scm")
(load "./runtime-cache.scm")
(load "./expander.scm")
(load "./bootstrap.gauche/mosh-stubs.scm")
(ex:expand-file "./r6rs.scm" "r6rs.gexp")