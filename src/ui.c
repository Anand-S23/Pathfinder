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
            case UI_WIDGET_button:
            {
                gs_vec4 color = {
                    0.6f + widget->t_hot * 0.4f - widget->t_active * 0.5f,
                    0.6f + widget->t_hot * 0.4f - widget->t_active * 0.5f,
                    0.6f + widget->t_hot * 0.4f - widget->t_active * 0.5f,
                    0.6f + widget->t_hot * 0.4f - widget->t_active * 0.5f,
                };
                
                // TODO: switch rendering call to gsi
                // DrawFilledRect(ui->buffer, widget->rect, color);
            } break;
            
            default:
            {
                printf("Error widget of type %d does not exist\n", widget->type);
            } break;
        }
    }
}

internal ui_option UIOption()
{
    ui_option opt = {0};
    opt.selected = UIIDNull();
    return opt;
}
        
internal ui_id UIOptionButton(ui *ui, ui_id current_selected, ui_id id, char *text,
                              gs_vec2 rect_br, gs_vec2 rect_tl,
                              gs_color_t color)
{

    return id;
}
