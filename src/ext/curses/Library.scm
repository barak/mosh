(mosh-curses
  c-function-table
  *internal*
  (plugin: mosh_curses)
  (libname: mosh-curses)
  #(ret name args)
  (int mcur_getch)
  (void mcur_acquire)
  (void mcur_release)
  (void mcur_refresh)
  (void mcur_mouse_enable)
  (void mcur_mouse_disable)
  (int mcur_lines)
  (int mcur_cols)
  (void mcur_locate (int int))
  (void mcur_cls)
  (int mcur_colors)
  (int mcur_color_pairs)
  (int mcur_color_configurable_p)
  (void mcur_color_content (int void* void* void*))
  (void mcur_pair_content (int void* void*))
  (void mcur_init_pair (int int int))
  (void mcur_init_color (int int int int))
  (int mcur_attrib_normal)
  (int mcur_attrib_standout)
  (int mcur_attrib_underline)
  (int mcur_attrib_reverse)
  (int mcur_attrib_blink)
  (int mcur_attrib_dim)
  (int mcur_attrib_bold)
  (int mcur_attrib_protect)
  (int mcur_attrib_invis)
  (int mcur_attrib_altcharset)
  (int mcur_attrib_color_pair (int))
  (void mcur_attron (int))
  (void mcur_attroff (int))
  (void mcur_attrset (int))
  (void mcur_print (char*))
  (int mcur_pdcurses)
  (void mcur_set_title (char*))
  (int mcur_key_resize)
  (int mcur_key_mouse)
  (void mcur_resize_term)
  )