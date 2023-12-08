#include "../image_processing/image_processing.h"
#include "../image_processing/utils.h"
#include "../sudoku_solver/solver_dec/solver_dec.h"

#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <err.h>

GdkPixbuf *pixbufImage;
GdkPixbuf *pixbufImageRotated;
int loaded_image = 0;
int state = 0;

GtkBuilder *builder;

GdkDisplay *display;
GdkScreen *screen;
gchar *filename;
gchar *filenameRotated;
GtkWidget *window = NULL;
GtkStack *stack;
GtkStack *stack_2;
GtkStack *stack_3;
SDL_Surface *global_preprocess = NULL;
SDL_Surface *global_rotated = NULL;

/**
 * @brief Convert a SDL_Surface to a GdkPixbuf
 *
 * @param surface: the SDL_Surface to convert
 * @return the GdkPixbuf
 */

GdkPixbuf *surface_to_pixbuf(SDL_Surface *surface)
{
    Uint32 src_format;
    Uint32 dst_format;

    GdkPixbuf *pixbuf;
    gboolean has_alpha;
    int rowstride;
    guchar *pixels;

    // select format
    src_format = surface->format->format;
    has_alpha = SDL_ISPIXELFORMAT_ALPHA(src_format);
    if (has_alpha)
        dst_format = SDL_PIXELFORMAT_RGBA32;
    else
        dst_format = SDL_PIXELFORMAT_RGB24;

    // create pixbuf
    pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, has_alpha, 8,
                            surface->w, surface->h);
    rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    pixels = gdk_pixbuf_get_pixels(pixbuf);

    // copy pixels
    SDL_LockSurface(surface);
    SDL_ConvertPixels(surface->w, surface->h, src_format,
                      surface->pixels, surface->pitch,
                      dst_format, pixels, rowstride);
    SDL_UnlockSurface(surface);

    return pixbuf;
}

/**
 * @brief Change the image of a GtkImage
 *
 * @param pixbuf: the new image
 * @param GtkimageID: the ID of the GtkImage
 */
void change_image(GdkPixbuf *pixbuf, char *GtkimageID)
{
    GtkImage *imageWidget =
        GTK_IMAGE(gtk_builder_get_object(builder, GtkimageID)); // get image

    size_t width = gdk_pixbuf_get_width(pixbuf);   // get width
    size_t height = gdk_pixbuf_get_height(pixbuf); // get height

    // resize image
    if (width > 650 || height > 650)
    {
        if (width > height)
        {
            double ratio = (double)width / (double)height; // get ratio
            width = 650;
            height = (size_t)round(width / ratio); // calculate new height
        }
        else
        {
            double ratio = (double)height / (double)width; // get ratio
            height = 650;
            width = (size_t)round(height / ratio); // calculate new width
        }

        pixbuf = gdk_pixbuf_scale_simple(pixbuf, width, height, GDK_INTERP_BILINEAR); // resize image
    }

    gtk_image_set_from_pixbuf(imageWidget, pixbuf); // set image
}

void set_logo_image(GdkPixbuf *pixbuf, char *GtkimageID)
{
    GtkImage *imageWidget =
        GTK_IMAGE(gtk_builder_get_object(builder, GtkimageID)); // get image

    size_t width = gdk_pixbuf_get_width(pixbuf);   // get width
    size_t height = gdk_pixbuf_get_height(pixbuf); // get height

    if (width > height)
    {
        double ratio = (double)width / (double)height; // get ratio
        width = 550;
        height = (size_t)round(width / ratio); // calculate new height
    }
    else
    {
        double ratio = (double)height / (double)width; // get ratio
        height = 550;
        width = (size_t)round(height / ratio); // calculate new width
    }

    pixbuf = gdk_pixbuf_scale_simple(pixbuf, width, height, GDK_INTERP_BILINEAR); // resize image

    gtk_image_set_from_pixbuf(imageWidget, pixbuf); // set image
}

/**
 * @brief Get the filename extension
 *
 * @param filename: the filename
 * @return the extension
 */
char *get_filename_ext(const char *filename)
{
    // get the filename extension
    char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return "";
    return dot + 1;
}

/**
 * @brief Resize a SDL_Surface
 *
 * @param surface: the SDL_Surface to resize
 * @param newWidth: the new width
 * @param newHeight: the new height
 * @return the resized SDL_Surface
 */
SDL_Surface *resize(SDL_Surface *surface, int newWidth, int newHeight)
{
    // Create a new SDL_Surface with the desired size
    SDL_Surface *resizedSurface = SDL_CreateRGBSurface(0, newWidth, newHeight, 32, 0, 0, 0, 0);

    // Fill the new surface with white color
    SDL_FillRect(resizedSurface, NULL, SDL_MapRGB(resizedSurface->format, 247, 247, 247));

    // Calculate the position to blit the original image
    int xOffset = (newWidth - surface->w) / 2;
    int yOffset = (newHeight - surface->h) / 2;

    // Blit the original image onto the new surface
    SDL_Rect sourceRect = {0, 0, surface->w, surface->h};
    SDL_Rect destRect = {xOffset, yOffset, surface->w, surface->h};
    SDL_BlitSurface(surface, &sourceRect, resizedSurface, &destRect);

    return resizedSurface;
}

/**
 * @brief Rotate a SDL_Surface
 *
 * @param source: the SDL_Surface to rotate
 * @param angle: the angle to rotate
 * @return the rotated SDL_Surface
 */
SDL_Surface *rotate_Surface(SDL_Surface *source, double angle)
{

    SDL_Surface *rotated = SDL_CreateRGBSurface(0, source->w * 1.5, source->h * 1.5, 32, 0, 0, 0, 0);

    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(rotated);
    SDL_SetRenderDrawColor(renderer, 247, 247, 247, 247); // white background
    SDL_RenderClear(renderer);

    // TODO: Fix rotation coordinates
    SDL_Rect dstRect = {(source->w / 2) * 0.8, (source->h / 2) * 0.8, source->w, source->h}; // center of the image
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, source);                   // Create texture from surface
    SDL_RenderCopyEx(renderer, texture, NULL, &dstRect, angle, NULL, SDL_FLIP_NONE);         // Render texture to screen
    SDL_DestroyTexture(texture);                                                             // Free the texture
    SDL_RenderPresent(renderer);                                                             // Update the screen

    SDL_DestroyRenderer(renderer); // Destroy the renderer
    return rotated;
}

SDL_Surface *cropImage(SDL_Surface *inputSurface)
{
    if (!inputSurface)
    {
        return NULL;
    }

    // Récupérer la largeur et la hauteur de la surface d'entrée
    int width = inputSurface->w;
    int height = inputSurface->h;

    // Déterminer les coordonnées de début et de fin du recadrage
    int startX = 0, startY = 0, endX = width - 1, endY = height - 1;

    // Trouver la coordonnée de début en X
    for (int x = 0; x < width; x++)
    {
        int allWhite = 1; // Supposons que toutes les lignes sont blanches

        for (int y = 0; y < height; y++)
        {
            Uint32 pixel = *((Uint32 *)inputSurface->pixels + y * width + x);
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, inputSurface->format, &r, &g, &b, &a);

            if (r != 247 || g != 247 || b != 247)
            {
                allWhite = 0; // La ligne n'est pas entièrement blanche
                break;
            }
        }

        if (allWhite)
        {
            startX = x + 1;
        }
        else
        {
            break;
        }
    }

    // Trouver la coordonnée de début en Y
    for (int y = 0; y < height; y++)
    {
        int allWhite = 1; // Supposons que toutes les colonnes sont blanches

        for (int x = 0; x < width; x++)
        {
            Uint32 pixel = *((Uint32 *)inputSurface->pixels + y * width + x);
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, inputSurface->format, &r, &g, &b, &a);

            if (r != 247 || g != 247 || b != 247)
            {
                allWhite = 0; // La colonne n'est pas entièrement blanche
                break;
            }
        }

        if (allWhite)
        {
            startY = y + 1;
        }
        else
        {
            break;
        }
    }

    // Trouver la coordonnée de fin en X
    for (int x = width - 1; x >= 0; x--)
    {
        int allWhite = 1; // Supposons que toutes les lignes sont blanches

        for (int y = 0; y < height; y++)
        {
            Uint32 pixel = *((Uint32 *)inputSurface->pixels + y * width + x);
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, inputSurface->format, &r, &g, &b, &a);

            if (r != 247 || g != 247 || b != 247)
            {
                allWhite = 0; // La ligne n'est pas entièrement blanche
                break;
            }
        }

        if (allWhite)
        {
            endX = x - 1;
        }
        else
        {
            break;
        }
    }

    // Trouver la coordonnée de fin en Y
    for (int y = height - 1; y >= 0; y--)
    {
        int allWhite = 1; // Supposons que toutes les colonnes sont blanches

        for (int x = 0; x < width; x++)
        {
            Uint32 pixel = *((Uint32 *)inputSurface->pixels + y * width + x);
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, inputSurface->format, &r, &g, &b, &a);

            if (r != 247 || g != 247 || b != 247)
            {
                allWhite = 0; // La colonne n'est pas entièrement blanche
                break;
            }
        }

        if (allWhite)
        {
            endY = y - 1;
        }
        else
        {
            break;
        }
    }

    // Calculer la nouvelle largeur et hauteur de la surface recadrée
    int newWidth = endX - startX + 1;
    int newHeight = endY - startY + 1;

    // Créer une nouvelle surface recadrée
    SDL_Surface *croppedSurface = SDL_CreateRGBSurface(0, newWidth, newHeight, 32, 0, 0, 0, 0);

    // Copier les pixels de la surface d'origine vers la nouvelle surface recadrée
    for (int y = 0; y < newHeight; y++)
    {
        for (int x = 0; x < newWidth; x++)
        {
            Uint32 pixel = *((Uint32 *)inputSurface->pixels + (y + startY) * width + (x + startX));
            *((Uint32 *)croppedSurface->pixels + y * newWidth + x) = pixel;
        }
    }

    return croppedSurface;
}

/**
 * @brief Rotate the image
 *
 * @param widget: the widget
 * @param data: the data
 */
void on_rotate(GtkWidget *widget, gpointer data)
{
    if (pixbufImage)
    {
        (void)widget;
        GtkScale *scale = GTK_SCALE(data);

        // get the degree
        double angle = gtk_range_get_value(GTK_RANGE(scale)); // get value of the scale
        printf("    🔄 Rotating image by %f degrees\n", angle);

        // rotate image
        SDL_Surface *image = IMG_Load(filename);                              // load image
        SDL_Surface *resized = resize(image, image->w * 1.5, image->h * 1.5); // resize image
        global_rotated = rotate_Surface(resized, angle);                      // rotate image
        global_rotated = cropImage(global_rotated);                           // crop image

        pixbufImageRotated = surface_to_pixbuf(global_rotated); // convert to pixbuf

        change_image(pixbufImageRotated, "selected_image2"); // change image

        SDL_FreeSurface(resized); // free surface
        SDL_FreeSurface(image);   // free surface
    }
    else
    {
        GtkWidget *dialog = gtk_message_dialog_new( // create error dialog
            GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "No image loaded");
        gtk_dialog_run(GTK_DIALOG(dialog)); // run dialog
        gtk_widget_destroy(dialog);         // destroy dialog
    }
}

/**
 * @brief Show the rotation page
 */
void rotate_screen()
{
    if (state != 0)
    {
        // create a gtk dialog to confirm the choice
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "If you continue, you will reset the process.\n Do you want to continue?");
        gtk_window_set_title(GTK_WINDOW(dialog), "Confirmation");
        gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_NO)
        {
            gtk_widget_destroy(dialog); // destroy dialog
            return;
        }
        gtk_widget_destroy(dialog); // destroy dialog
    }
    if (pixbufImage)
        change_image(pixbufImage, "selected_image2");
    state = 0;
    gtk_stack_set_visible_child_name(stack_2, "rotationPage");
    gtk_stack_set_visible_child_name(stack_3, "lower_panel3");
}

/**
 * @brief Load the image
 *
 * @param file_chooser: the file chooser
 * @param data: the data
 */
void on_file_set(GtkFileChooserButton *file_chooser, gpointer data)
{
    (void)data;

    // select filename and update image
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser)); // get filename
    char *ext = get_filename_ext(filename);                                   // get extension

    // check if file is an image
    if (strcmp(ext, "png") == 0 || strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0 || strcmp(ext, "bmp") == 0)
    {
        GError *error = NULL;

        pixbufImage = gdk_pixbuf_new_from_file(filename, &error); // load image
        global_preprocess = IMG_Load(filename);                   // load image

        if (error)
        {
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
        gtk_widget_destroy(dialog);         // destroy dialog
    }
}

void on_confirm_values(GtkWidget *widget, gpointer data)
{
    (void)widget;
    (void)data;
    GtkWidget *grid = GTK_WIDGET(gtk_builder_get_object(builder, "9x9Grid"));
    // Get all the entries by list of the child of the grid
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(grid));
    int *values = malloc(sizeof(int) * 81);
    int i = 0;
    for (iter = children; iter != NULL; iter = g_list_next(iter))
    {
        GtkWidget *entry = GTK_WIDGET(iter->data);
        const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
        if (strlen(text) == 0)
            values[i] = 0;
        else
            values[i] = atoi(text);

        i++;
    }

    FILE *file = fopen("sudoku.txt", "w");
    if (file == NULL)
        errx(1, "Error opening file");
    for (int i = 8; i >= 0; i--)
    {
        for (int j = 8; j >= 0; j--)
        {
            int index = i * 9 + j;
            if (values[index] == 0)
                fprintf(file, ".");
            else
                fprintf(file, "%d", values[index]);
            if (j % 3 == 0 && j != 0)
                fprintf(file, " ");
        }
        fprintf(file, "\n");
        if (i % 3 == 0 && i != 0)
            fprintf(file, "\n");
    }

    fclose(file);
    int **tempgrid = readGridFromFile("sudoku.txt");
    int oldgrid[N][N];
    int newgrid[N][N];
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            oldgrid[i][j] = tempgrid[i][j];
            newgrid[i][j] = tempgrid[i][j];
        }
    }

    solver(newgrid, 0, 0);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d", oldgrid[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d", newgrid[i][j]);
        }
        printf("\n");
    }
    SDL_Surface *resolve_grid = createSudokuImage(oldgrid, newgrid, 96 * 9, "../sudoku_solver/assets/");
    change_image(surface_to_pixbuf(resolve_grid), "result_image");
    gtk_stack_set_visible_child_name(stack_2, "page_result"); // show the result page
}

/**
 * @brief Go back to the main page
 *
 * @param widget: the widget
 * @param data: the data
 */
void on_cancel_rotate(GtkWidget *widget, gpointer data)
{
    (void)widget;
    (void)data;
    state = 0;
    if (pixbufImage)
        change_image(pixbufImage, "selected_image");

    gtk_stack_set_visible_child_name(stack_2, "page1"); // show page 2
    gtk_stack_set_visible_child_name(stack_3, "lower_panel2");
}

void on_click_values(GtkWidget *widget, gpointer data)
{
    (void)widget;
    (void)data;
    // set the label of the button
    gtk_button_set_label(GTK_BUTTON(gtk_builder_get_object(builder, "valueButton")), "Confirm values");

    // change the signal of the button
    g_signal_handlers_disconnect_by_func(G_OBJECT(gtk_builder_get_object(builder, "valueButton")), G_CALLBACK(on_click_values), NULL);
    g_signal_connect(G_OBJECT(gtk_builder_get_object(builder, "valueButton")), "clicked", G_CALLBACK(on_confirm_values), NULL);

    gtk_stack_set_visible_child_name(stack_2, "page_confirm");
}

/**
 * @brief Save the rotated image and go back to the main page
 *
 * @param widget: the widget
 * @param data: the data
 */
void on_auto_rotate(GtkWidget *widget, gpointer data)
{
    // TODO: AUTOROTATE
}

void on_step_by_step(GtkWidget *widget, gpointer data)
{
    if (!filename)
    {
        GtkWidget *dialog = gtk_message_dialog_new( // create error dialog
            GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "No image loaded");
        gtk_dialog_run(GTK_DIALOG(dialog)); // run dialog
        gtk_widget_destroy(dialog);         // destroy dialog
        return;
    }
    if (state == 0)
    {
        SDL_Surface *surface = convertToGrayscale(global_preprocess);
        state++;
        GdkPixbuf *pixbuf = surface_to_pixbuf(surface);
        change_image(pixbuf, "result_image");
        gtk_stack_set_visible_child_name(stack_2, "page_result"); // show the result page
    }
}

void skip_to_the_result(GtkWidget *widget, gpointer data)
{
    if (!filename)
    {
        GtkWidget *dialog = gtk_message_dialog_new( // create error dialog
            GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "No image loaded");
        gtk_dialog_run(GTK_DIALOG(dialog)); // run dialog
        gtk_widget_destroy(dialog);         // destroy dialog
        return;
    }
}

/**
 * @brief Show the right page
 *
 * @param widget: the widget
 * @param data: the data
 */
void show_page(GtkWidget *widget, gpointer data)
{
    // Avoid warning
    (void)widget;

    GtkWidget *page = data;
    gtk_stack_set_visible_child(stack, page);
    gtk_stack_set_visible_child_name(stack_3, "lower_panel2");
}

/**
 * @brief Change the panel
 *
 * @param widget: the widget
 * @param data: the data
 */
void change_panel(GtkWidget *widget, gpointer data)
{
    // Avoid warning
    (void)widget;

    GtkWidget *page = data;
    gtk_stack_set_visible_child(stack_2, page);
}

/**
 * @brief Quit the program and free memory
 */
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

/**
 * @brief Scale a GtkImage to the given width and height
 *
 * @param image: the GtkImage to scale
 * @param new_width: the new width
 * @param new_height: the new height
 */
void scale_image(GtkImage *image, int new_width, int new_height)
{
    // Get the original pixbuf from the image
    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(image);

    // Scale the pixbuf
    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, new_width, new_height, GDK_INTERP_HYPER);

    // Set the scaled pixbuf to the image
    gtk_image_set_from_pixbuf(image, scaled_pixbuf);

    // Unreference the scaled pixbuf as it's no longer needed
    g_object_unref(scaled_pixbuf);
}

/**
 * @brief Resize all the widgets
 *
 * @param widget: the widget
 * @param allocation: the allocation
 * @param user_data: the user data
 */
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
    if (!builder)
    {
        g_error("Failed to create GtkBuilder");
        return 1;
    }

    filename = g_build_filename("main.glade", NULL); // build filename
    if (!filename)
    {
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

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "styledefou.css", NULL);
    screen = gdk_screen_get_default();
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window")); // get window
    if (!window)
    {
        g_error("Failed to get window");
        g_object_unref(builder);
        return 1;
    }

    gtk_builder_connect_signals(builder, NULL); // connect signals

    // Set title
    gtk_window_set_title(GTK_WINDOW(window), "Optical Cramptés Recognition");

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("C_de_crampte.png", &error);
    set_logo_image(pixbuf, "startImage");

    stack = GTK_STACK(gtk_builder_get_object(builder, "window_pages"));
    stack_2 = GTK_STACK(gtk_builder_get_object(builder, "upper_panel"));
    stack_3 = GTK_STACK(gtk_builder_get_object(builder, "lower_stack"));

    // on resize window
    g_signal_connect(window, "size-allocate", G_CALLBACK(on_resize), NULL);

    GtkWidget *scale = GTK_WIDGET(gtk_builder_get_object(builder, "rotate_scale"));
    gtk_range_set_range(GTK_RANGE(scale), 0, 360); // set range
    gtk_range_set_value(GTK_RANGE(scale), 0);      // set default value

    // load UI
    gtk_window_fullscreen(GTK_WINDOW(window)); // fullscreen
    gtk_widget_show_all(window);               // show window
    gtk_main();                                // start main loop

    return EXIT_SUCCESS;
}
