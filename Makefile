gtk_app: gtk_app.c style.css
	gcc `pkg-config --cflags gtk4` -o gtk_app gtk_app.c `pkg-config --libs gtk4`

clean:
	rm gtk_app
