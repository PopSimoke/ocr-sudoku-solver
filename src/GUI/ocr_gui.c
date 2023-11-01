#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>


GdkPixbuf *pixbufImage;
GdkPixbuf *pixbufImageRotated;
int loaded_image = 0;

GtkBuilder *builder;
gchar *filename;
gchar *filenameRotated;
GtkWidget *window = NULL;
GtkStack *stack;
GtkStack *stack_2;
SDL_Surface *global_rotated = NULL;
 
GdkPixbuf* surface_to_pixbuf(SDL_Surface* surface) 
{
    GdkPixbuf* pixbuf = NULL;
    if (surface != NULL) 
    {
        pixbuf = gdk_pixbuf_new_from_data(surface->pixels,
                                          GDK_COLORSPACE_RGB,
                                          surface->format->BytesPerPixel == 4,
                                          8,
                                          surface->w,
                                          surface->h,
                                          surface->pitch,
                                          NULL,
                                          NULL);
    }

    return pixbuf;
}


void change_image(GdkPixbuf *pixbuf, char *GtkimageID)
{
    GtkImage *imageWidget =
        GTK_IMAGE(gtk_builder_get_object(builder, GtkimageID)); // get image

    gtk_image_set_from_pixbuf(imageWidget, pixbuf); 
}

char *get_filename_ext(const char *filename)
{
    // get the filename extension
    char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return "";
    return dot + 1;
}

SDL_Surface* resize(SDL_Surface* surface, int newWidth, int newHeight) 
{
    // Create a new SDL_Surface with the desired size
    SDL_Surface* resizedSurface = SDL_CreateRGBSurface(0, newWidth, newHeight, 32, 0, 0, 0, 0);

    // Fill the new surface with white color
    SDL_FillRect(resizedSurface, NULL, SDL_MapRGB(resizedSurface->format, 255, 255, 255));

    // Calculate the position to blit the original image
    int xOffset = (newWidth - surface->w) / 2;
    int yOffset = (newHeight - surface->h) / 2;

    // Blit the original image onto the new surface
    SDL_Rect sourceRect = {0, 0, surface->w, surface->h};
    SDL_Rect destRect = {xOffset, yOffset, surface->w, surface->h};
    SDL_BlitSurface(surface, &sourceRect, resizedSurface, &destRect);

    return resizedSurface;
}

SDL_Surface* rotate(SDL_Surface* source, double angle) {
    SDL_Surface* rotated = SDL_CreateRGBSurface(0, source->w*1.5, source->h*1.5, 32, 0, 0, 0, 0);

    SDL_Renderer* renderer = SDL_CreateSoftwareRenderer(rotated);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Fond blanc
    SDL_RenderClear(renderer);

    SDL_Rect dstRect = {150, 150, source->w, source->h};
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, source);
    SDL_RenderCopyEx(renderer, texture, NULL, &dstRect, angle, NULL, SDL_FLIP_NONE);
    SDL_DestroyTexture(texture);  // Libérer la texture
    SDL_RenderPresent(renderer);

    SDL_DestroyRenderer(renderer);
    return rotated;
}



void on_rotate(GtkWidget *widget, gpointer data)
{

        if (pixbufImage)
        {
            (void)widget;
            GtkScale *scale = GTK_SCALE(data);

            // get the degree
            double angle = gtk_range_get_value(GTK_RANGE(scale));
            printf("    🔄 Rotating image by %f degrees\n", angle);

            // rotate image
            SDL_Surface *image = IMG_Load(filename);
            SDL_Surface *resized = resize(image, image->w*1.05, image->h*1.05);
            global_rotated = rotate(resized, angle);
            
            pixbufImageRotated = surface_to_pixbuf(global_rotated);

            change_image(pixbufImageRotated, "selected_image2");

            SDL_FreeSurface(resized);
            SDL_FreeSurface(image);
        }
        else
        {
            GtkWidget *dialog = gtk_message_dialog_new(
                GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "No image loaded");
            gtk_dialog_run(GTK_DIALOG(dialog)); // run dialog
            gtk_widget_destroy(dialog); // destroy dialog
        }
        
}

void rotate_screen()
{
    if (pixbufImageRotated)
        change_image(pixbufImageRotated, "selected_image2");
    else if (pixbufImage)
        change_image(pixbufImage, "selected_image2");
    
    gtk_stack_set_visible_child_name(stack_2, "rotationPage");   
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

        // update label
        GtkLabel *label =
            GTK_LABEL(gtk_builder_get_object(builder, "image_path"));
        gtk_label_set_text(label, filename);

        // Display image
        const char *visible_child_name = gtk_stack_get_visible_child_name(GTK_STACK(stack_2));
        if (visible_child_name == NULL || strcmp(visible_child_name, "page1") == 0)
        {
            change_image(pixbufImage, "selected_image");
            gtk_stack_set_visible_child_name(stack_2, "page2"); // show page 2
        }
        else if (strcmp(visible_child_name, "page2") == 0)
        {
            change_image(pixbufImage, "selected_image");
        }
        else if (strcmp(visible_child_name, "rotationPage") == 0)
        {
            change_image(pixbufImage, "selected_image2");
        }

        loaded_image = 1;
        
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

void on_cancel_rotate(GtkWidget *widget, gpointer data)
{
    (void)widget;
    (void)data;
    
    if (pixbufImage)
        change_image(pixbufImage, "selected_image");

    gtk_stack_set_visible_child_name(stack_2, "page2"); // show page 2
}

void on_save_rotate(GtkWidget *widget, gpointer data)
{
    if (pixbufImageRotated)
    {
        (void)widget;
        (void)data;

        // save image
        char *ext = get_filename_ext(filename);
        filenameRotated = g_strdup_printf("%s_rotated.%s", filename, ext);
        
        GError *error = NULL;
        gdk_pixbuf_save(pixbufImageRotated, filenameRotated, ext, &error, NULL);
        printf("    💾 Saving image at %s\n", filenameRotated);

        if (error) {
            g_printerr("Error saving file: %s\n", error->message);
            g_clear_error(&error);
            return;
        }

        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Image saved at %s", filenameRotated);
        gtk_dialog_run(GTK_DIALOG(dialog)); // run dialog
        gtk_widget_destroy(dialog); // destroy dialog

        change_image(pixbufImageRotated, "selected_image");
    }
    else
        change_image(pixbufImage, "selected_image");

    gtk_stack_set_visible_child_name(stack_2, "page2"); // show page 2
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
    
    // Free all memory
    g_object_unref(builder);
    if (filename)
        g_free(filename);
    if (filenameRotated)
        g_free(filenameRotated);
    if (pixbufImage)
        g_object_unref(pixbufImage);
    if (pixbufImageRotated)
        g_object_unref(pixbufImageRotated);
    if (global_rotated) 
        SDL_FreeSurface(global_rotated);
    
    // quit the program
    gtk_main_quit();

    
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

    GtkWidget *scale = GTK_WIDGET(gtk_builder_get_object(builder, "rotate_scale"));
    gtk_range_set_range(GTK_RANGE(scale), 0, 360); // set range
    gtk_range_set_value(GTK_RANGE(scale), 0); // set default value


    // load UI
    gtk_window_fullscreen(GTK_WINDOW(window)); // fullscreen
    gtk_widget_show_all(window); // show window
    gtk_main(); // start main loop

    return EXIT_SUCCESS;
}
                                                            
