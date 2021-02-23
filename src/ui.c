#include "ui.h"

internal ui_id UIIDNull()
{
    ui_id id = { -1, -1 };
    return id;
}

internal ui_id UIIDInit(u32 primary, u32 secondary)
{
    ui_id id = { primary, secondary };
    return id;
}

internal b32 UIIDEqual(ui_id id1, ui_id id2)
{
    return (id1.primary == id2.primary &&
            id1.secondary == id2.secondary);
}

internal void UIBeginFrame(ui *ui, gs_immediate_draw_t *renderer, input *input)
{
    ui->renderer = renderer;

    ui->mouse_x          = input->mouse_x;
    ui->mouse_y          = input->mouse_y;
    ui->left_mouse_down  = input->left_mouse_down;
    ui->right_mouse_down = input->right_mouse_down;

    ui->widget_count = 0;
}

internal void UIEndFrame(ui *ui)
{
    for(u32 i = 0; i < ui->widget_count; ++i)
    {
        ui_widget *widget = ui->widgets + i;
        
        widget->t_hot    += ((f32)(!!UIIDEqual(ui->hot, widget->id)) - widget->t_hot);
        widget->t_active += ((f32)(!!UIIDEqual(ui->active, widget->id)) - widget->t_active);
        
        switch(widget->type)
        {
            // TODO: Add text for the buttons
            case UI_WIDGET_button:
            {
                gs_color_t color;

                if (widget->t_hot && widget->t_active)
                {
                    color = gs_color(60, 60, 60, 255);
                }
                else if (widget->t_hot)
                {
                    color = gs_color(150, 150, 150, 255);
                }
                else if (widget->t_active)
                {
                    color = gs_color(45, 45, 45, 255);
                }
                else
                {
                    color = gs_color(100, 100, 100, 255);
                }
                
                gsi_rectv(ui->renderer, widget->rect_br, widget->rect_tl,
                          color, GS_GRAPHICS_PRIMITIVE_TRIANGLES);
            } break;
            
            case UI_WIDGET_custom_button:
            {
                gs_color_t color;

                if (widget->t_hot)
                {
                    color = gs_color(150, 150, 150, 255);
                }
                else if (widget->t_active)
                {
                    color = gs_color(100, 100, 100, 255);
                }
                else
                {
                    color = widget->color;
                }
                
                gsi_rectv(ui->renderer, widget->rect_br, widget->rect_tl,
                          color, GS_GRAPHICS_PRIMITIVE_TRIANGLES);
            } break;
            
            case UI_WIDGET_option_button:
            {
                gs_color_t color;

                if (widget->option.is_active)
                {
                    color = widget->color;
                }
                else
                {
                    if (widget->t_hot)
                    {
                        color = gs_color(150, 150, 150, 255);
                    }
                    else if (widget->t_active)
                    {
                        color = gs_color(50, 50, 50, 255);
                    }
                    else
                    {
                        color = gs_color(100, 100, 100, 255);
                    }
                }
                
                gsi_rectv(ui->renderer, widget->rect_br, widget->rect_tl,
                          color, GS_GRAPHICS_PRIMITIVE_TRIANGLES);
            } break;

            default:
            {
                printf("Error widget of type %d does not exist\n", widget->type);
            } break;
        }
    }
}

internal b32 UIButton(ui *ui, ui_id id, char *text,
                      gs_vec2 rect_br, gs_vec2 rect_tl)
{
    // Assert(ui->widget_count < UI_MAX_WIDGETS);
    b32 is_triggered = 0;
    
    b32 cursor_is_over = (ui->mouse_x >= rect_tl.x &&
                          ui->mouse_x <= rect_br.x &&
                          ui->mouse_y >= rect_tl.y &&
                          ui->mouse_y <= rect_br.y);
    
    if (!UIIDEqual(ui->hot, id) && cursor_is_over)
    {
        ui->hot = id;
    }
    else if (UIIDEqual(ui->hot, id) && !cursor_is_over)
    {
        ui->hot = UIIDNull();
    }
    
    if (UIIDEqual(ui->active, id))
    {
        if(!ui->left_mouse_down)
        {
            is_triggered = UIIDEqual(ui->hot, id);
            ui->active = UIIDNull();
        }
    }
    else
    {
        if (UIIDEqual(ui->hot, id))
        {
            if (ui->left_mouse_down)
            {
                ui->active = id;
            }
        }
    }
    
    ui_widget *widget = ui->widgets + ui->widget_count++;
    widget->type = UI_WIDGET_button;
    widget->id = id;
    widget->rect_br = rect_br;
    widget->rect_tl = rect_tl;

    return is_triggered;
}

internal b32 UICustomButton(ui *ui, ui_id id, char *text, gs_vec2 rect_br,
                            gs_vec2 rect_tl, gs_color_t color)
{
    // Assert(ui->widget_count < UI_MAX_WIDGETS);
    b32 is_triggered = 0;
    
    b32 cursor_is_over = (ui->mouse_x >= rect_tl.x &&
                          ui->mouse_x <= rect_br.x &&
                          ui->mouse_y >= rect_tl.y &&
                          ui->mouse_y <= rect_br.y);
    
    if (!UIIDEqual(ui->hot, id) && cursor_is_over)
    {
        ui->hot = id;
    }
    else if (UIIDEqual(ui->hot, id) && !cursor_is_over)
    {
        ui->hot = UIIDNull();
    }
    
    if (UIIDEqual(ui->active, id))
    {
        if(!ui->left_mouse_down)
        {
            is_triggered = UIIDEqual(ui->hot, id);
            ui->active = UIIDNull();
        }
    }
    else
    {
        if (UIIDEqual(ui->hot, id))
        {
            if (ui->left_mouse_down)
            {
                ui->active = id;
            }
        }
    }
    
    ui_widget *widget = ui->widgets + ui->widget_count++;
    widget->type = UI_WIDGET_custom_button;
    widget->id = id;
    widget->rect_br = rect_br;
    widget->rect_tl = rect_tl;
    widget->color = color;

    return is_triggered;
}

internal ui_option UIOption(gs_color_t color, gs_vec2 size)
{
    ui_option option = {0};
    option.color = color;
    option.size = size;

    return option;
}

internal ui_id UIOptionButton(ui *ui, ui_option *parent, ui_id id,
                              char *text, gs_vec2 origin)
{
    // Assert(ui->widget_count < UI_MAX_WIDGETS);
    
    b32 is_triggered = 0;
    
    b32 cursor_is_over = (ui->mouse_x >= origin.x &&
                          ui->mouse_x <= origin.x + parent->size.x &&
                          ui->mouse_y >= origin.y &&
                          ui->mouse_y <= origin.y + parent->size.y);
    
    if (!UIIDEqual(ui->hot, id) && cursor_is_over)
    {
        ui->hot = id;
    }
    else if (UIIDEqual(ui->hot, id) && !cursor_is_over)
    {
        ui->hot = UIIDNull();
    }
    
    if (UIIDEqual(ui->active, id))
    {
        if(!ui->left_mouse_down)
        {
            is_triggered = UIIDEqual(ui->hot, id);

            if (is_triggered)
            {
                parent->selected = id;
            }

            ui->active = UIIDNull();
        }
    }
    else
    {
        if (UIIDEqual(ui->hot, id))
        {
            if (ui->left_mouse_down)
            {
                ui->active = id;
            }
        }
    }
    
    ui_widget *widget = ui->widgets + ui->widget_count++;
    widget->type = UI_WIDGET_option_button;
    widget->id = id;
    widget->rect_br = gs_v2(origin.x + parent->size.x,
                            origin.y + parent->size.y);
    widget->rect_tl = origin;
    widget->option.is_active = UIIDEqual(id, parent->selected);
    widget->color = parent->color;

    return id;
}
