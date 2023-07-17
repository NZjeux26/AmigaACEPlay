#include <ace/

int main(void)
{
  // Create a lowres window.
  ace_window_t *window = ace_window_create(800, 600, 0);

  // Draw ten lines of different colors in different directions.
  for (int i = 0; i < 10; i++)
  {
    int x1 = rand() % 800;
    int y1 = rand() % 600;
    int x2 = rand() % 800;
    int y2 = rand() % 600;
    int color = rand() % 256;

    ace_line_draw(window, x1, y1, x2, y2, color);
  }

  // Close the window on a left mouse click.
  ace_window_event_handler(window, ACE_EVENT_MOUSE_LEFT_DOWN, ace_window_close);

  // Wait for the user to close the window.
  ace_window_wait(window);

  // Cleanup.
  ace_window_destroy(window);

  return 0;
}
