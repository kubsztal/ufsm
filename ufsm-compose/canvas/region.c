#include <math.h>
#include <ufsm/model/ufsmm.h>
#include <ufsm/model/model.h>
#include <ufsm/model/stack.h>

#include "canvas/view.h"

int ufsmm_canvas_render_region(cairo_t *cr, struct ufsmm_region *region)
{
    double x, y, w, h;
    double dashes[] = {10.0,  /* ink */
                       10.0};  /* skip */

    double y_offset = 0.0;

    if (region->draw_as_root)
        return UFSMM_OK;

    ufsmm_get_region_absolute_coords(region, &x, &y, &w, &h);

    if (region->parent_state)
        y_offset = region->parent_state->region_y_offset;

    cairo_save (cr);
    cairo_set_font_size (cr, 18);
    ufsmm_color_set(cr, UFSMM_COLOR_NORMAL);
    cairo_move_to (cr, x + 10, y + y_offset + 25);
    cairo_show_text (cr, region->name);
    cairo_restore(cr);

    if (region->focus) {
        cairo_save(cr);
        ufsmm_color_set(cr, UFSMM_COLOR_ACCENT);
        cairo_set_dash (cr, dashes, 2, 0);
        cairo_set_line_width (cr, 2);
        cairo_rectangle (cr, x, y + y_offset, w, h - y_offset);
        cairo_stroke (cr);
        cairo_restore (cr);
    }

    /* Possibly render region 'off-page' symbol */
    if (region->off_page && !region->draw_as_root) {
        cairo_save(cr);
        ufsmm_color_set(cr, UFSMM_COLOR_NORMAL);
        /* Render two, filled circles */
        cairo_new_sub_path(cr);
        cairo_translate(cr, x + w - 50, y - 20 + h);
        cairo_arc(cr, 0, 0, 5, 0, 2 * M_PI);
        cairo_translate(cr, 20, 0);
        cairo_arc(cr, 0, 0, 5, 0, 2 * M_PI);
        cairo_fill(cr);
        cairo_close_path(cr);
        cairo_restore(cr);
        /* and the line between them */
        cairo_save(cr);
        cairo_move_to (cr,  x + w - 50, y + h - 20);
        cairo_line_to(cr, x + w - 30, y + h - 20);
        ufsmm_color_set(cr, UFSMM_COLOR_NORMAL);
        cairo_set_line_width (cr, 2.0);
        cairo_stroke(cr);
        cairo_restore(cr);
    }

    /* Render resize boxes */
    if (region->focus) {
        cairo_save(cr);
        ufsmm_color_set(cr, UFSMM_COLOR_ACCENT);
        cairo_rectangle (cr, x + w/2 - 5, y - 5, 10, 10);
        cairo_rectangle (cr, x + w/2 - 5, y + h - 5, 10, 10);
        cairo_fill(cr);
        cairo_restore(cr);
    }

    return 0;
}
