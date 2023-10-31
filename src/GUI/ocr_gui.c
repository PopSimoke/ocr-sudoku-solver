#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

GdkPixbuf *pixbufImage;
int loaded_image = 0;

GtkBuilder *builder;
gchar *filename;
GtkWidget *window = NULL;
GtkStack *stack;
GtkStack *stack_2;

double clamp(double d, double min, double max) {
  const double t = d < min ? min : d;
  return t > max ? max : t;
}

GdkPixbuf *surface_to_pixbuf(SDL_Surface *surface)
{
    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,
                                       SDL_ISPIXELFORMAT_ALPHA(surface->format->format),
                                       8, surface->w, surface->h);

    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);

    for (int y = 0; y < surface->h; y++)
    {
        for (int x = 0; x < surface->w; x++)
        {
            Uint8 r, g, b, a;
            Uint32 pixel = ((Uint32 *)surface->pixels)[y * surface->w + x];
            SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

            pixels[y * rowstride + x * 4 + 0] = r;
            pixels[y * rowstride + x * 4 + 1] = g;
            pixels[y * rowstride + x * 4 + 2] = b;
            pixels[y * rowstride + x * 4 + 3] = a;
        }
    }

    return pixbuf;
}


void set_selected_image(GdkPixbuf *pixbuf, char *GtkimageID)
{
    GtkImage *imageWidget =
        GTK_IMAGE(gtk_builder_get_object(builder, GtkimageID)); // get image

    GtkStack *panel = GTK_STACK(gtk_builder_get_object(builder, "upper_panel"));

    int width = clamp(gtk_widget_get_allocated_width(GTK_WIDGET(panel)), 0,
                      1000); // TODO: fix this
    int height =
        clamp(gtk_widget_get_allocated_height(GTK_WIDGET(panel)), 0, 1000);

    // get image size
    int image_width = gdk_pixbuf_get_width(pixbuf);
    int image_height = gdk_pixbuf_get_height(pixbuf);

    // get scale factor
    double scale_factor = 1;
    if (image_width > width || image_height > height)
    {
        scale_factor = (double)width / image_width;
        if (scale_factor * image_height > height)
            scale_factor = (double)height / image_height;
    }
    int new_width = image_width * scale_factor;
    int new_height = image_height * scale_factor;

    printf("%f %d %d\n", scale_factor, width, height);
    // resize the image
    GdkPixbuf *resized_image = gdk_pixbuf_scale_simple(
        pixbuf, new_width, new_height, GDK_INTERP_BILINEAR); // resize image

    // set the image
    gtk_image_set_from_pixbuf(imageWidget, resized_image);

    // free
    g_object_unref(pixbuf);
    g_object_unref(resized_image);
}

void change_image(GdkPixbuf *pixbuf, char *GtkimageID)
{
    set_selected_image(pixbuf, GtkimageID);
}



char *get_filename_ext(const char *filename)
{
    // get the filename extension
    char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return "";
    return dot + 1;
}

void on_file_set(GtkFileChooserButton *file_chooser, gpointer data)
{
    (void)data;

    // select filename and update image
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser));
    char *ext = get_filename_ext(filename);
    

    // check if file is an image
    if (strcmp(ext, "png") == 0 || strcmp(ext, "jpg") == 0
        || strcmp(ext, "jpeg") == 0 || strcmp(ext, "bmp") == 0)
    {
        GError *error = NULL;

        pixbufImage = gdk_pixbuf_new_from_file(filename, &error);

        if (error) {
            g_printerr("Error loading file: %s\n", error->message);
            g_clear_error(&error);
            return;
        }

        printf("    🎨 Loaded %s\n", filename);

        // Display image
        change_image(pixbufImage, "selected_image");

        loaded_image = 1;

        // update label
        GtkLabel *label =
            GTK_LABEL(gtk_builder_get_object(builder, "image_path"));
        gtk_label_set_text(label, filename);

        gtk_stack_set_visible_child_name(stack_2, "page2"); // show page 2
        
    }
    else
    {   
        // display error message
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "File is not an image");
        gtk_dialog_run(GTK_DIALOG(dialog)); // run dialog
        gtk_widget_destroy(dialog); // destroy dialog
    }
}



void set_lowerPannel_status(gboolean status)
{
    // set the  lower pannel status
    GtkWidget *lowerPannel =
        GTK_WIDGET(gtk_builder_get_object(builder, "lower_panel"));
    gtk_widget_set_sensitive(lowerPannel, status);
}


void show_page(GtkWidget *widget, gpointer data)
{
    // Avoid warning
    (void)widget;

    GtkWidget *page = data;
    gtk_stack_set_visible_child(stack, page);
}

void change_panel(GtkWidget *widget, gpointer data)
{
    // Avoid warning
    (void)widget;

    GtkWidget *page = data;
    gtk_stack_set_visible_child(stack_2, page);
}



void open_file(GtkWidget *widget, gpointer data)
{
    (void)widget;
    // open image in explorer
    char *path = data;
    char command[100] = "xdg-open ";
    strcat(command, path);
    if (system(command))
    {
        printf("Error: Could not open file\n");

        // display error message
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Could not open file");

        gtk_dialog_run(GTK_DIALOG(dialog)); // run dialog
        gtk_widget_destroy(dialog); // destroy dialog
    }
}

void quit()
{
    // quit the program
    gtk_main_quit();

    // Free builder
    g_object_unref(builder);
    g_object_unref(pixbufImage);
}


void scale_image(GtkImage *image, int new_width, int new_height) {
    // Get the original pixbuf from the image
    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(image);

    // Scale the pixbuf
    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, new_width, new_height, GDK_INTERP_HYPER);

    // Set the scaled pixbuf to the image
    gtk_image_set_from_pixbuf(image, scaled_pixbuf);

    // Unreference the scaled pixbuf as it's no longer needed
    g_object_unref(scaled_pixbuf);
}

 
void on_resize(GtkWidget *widget, GdkRectangle *allocation, gpointer user_data)
{
    return;
    (void)user_data;
    (void)widget;
    
    int new_width = allocation->width;
    int new_height = allocation->height;

    // Scale the image
    scale_image(GTK_IMAGE(gtk_builder_get_object(builder, "selected_image")), new_width, new_height);

}


int main(int argc, char *argv[])
{
    GError *error = NULL;
    gchar *filename = NULL;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new(); // initialize the builder
    if (!builder) {
        g_error("Failed to create GtkBuilder");
        return 1;
    }

    filename = g_build_filename("main.glade", NULL); // build filename
    if (!filename) {
        g_error("Failed to build filename");
        g_object_unref(builder);
        return 1;
    }

    if (!gtk_builder_add_from_file(builder, filename, &error)) // load file
    {
        g_warning("%s", error->message);
        g_free(filename);
        g_error_free(error);
        g_object_unref(builder);
        return 1;
    }
    g_free(filename); // free filename

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window")); // get window
    if (!window) {
        g_error("Failed to get window");
        g_object_unref(builder);
        return 1;
    }

    gtk_builder_connect_signals(builder, NULL); // connect signals

    // Set title
    gtk_window_set_title(GTK_WINDOW(window), "Optical Cramptés Recognition");

    stack = GTK_STACK(gtk_builder_get_object(builder, "window_pages"));
    stack_2 = GTK_STACK(gtk_builder_get_object(builder, "upper_panel"));

    // on resize window
    g_signal_connect(window, "size-allocate", G_CALLBACK(on_resize), NULL);

    // load UI
    gtk_window_fullscreen(GTK_WINDOW(window)); // fullscreen
    gtk_widget_show_all(window); // show window
    gtk_main(); // start main loop

    return EXIT_SUCCESS;
}
                                                            
