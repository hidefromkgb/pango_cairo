#include <pango/pangocairo.h>

void text(cairo_t *temp, int diam, int edge, char *line, char *font) {
    const double icnt = 1.0 / edge;
    PangoFontDescription *desc;
    PangoLayout *lpgo;
    int iter, xdim;

    pango_layout_set_text(lpgo = pango_cairo_create_layout(temp), line, -1);
    desc = pango_font_description_from_string(font);
    pango_layout_set_font_description(lpgo, desc);
    pango_font_description_free(desc);
    pango_layout_get_pixel_size(lpgo, &xdim, &iter);
    cairo_translate(temp, 0.5 * diam, 0.5 * diam);

    for (iter = 1.0 / icnt; iter; iter--) {
        cairo_set_source_rgb(temp, icnt * iter, 0.0, 1.0 - icnt * iter);
        cairo_rotate(temp, icnt * G_PI * 2.0);
        pango_cairo_update_layout(temp, lpgo);
        cairo_move_to(temp, -0.5 * xdim, -0.5 * diam);
        pango_cairo_show_layout(temp, lpgo);
    }
    g_object_unref(lpgo);
}

int main(int argc, char **argv) {
    const int diam = 300;
    cairo_surface_t *surf;
    cairo_t *temp;

    if (argc != 2)
        g_printerr("Usage: %s OUTPUT_FILE.png\n", argv[0]);
    else {
        surf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, diam, diam);
        cairo_set_source_rgb(temp = cairo_create(surf), 1.0, 1.0, 1.0);
        cairo_paint(temp);
        text(temp, diam, 7, "-Line-", "Sans Bold 30");
        cairo_destroy(temp);
        if (cairo_surface_write_to_png(surf, argv[1]) != CAIRO_STATUS_SUCCESS)
            g_printerr("Could not save png to '%s'\n", argv[1]);
        cairo_surface_destroy(surf);
    }
    return 0;
}
