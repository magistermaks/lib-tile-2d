package net.darktree.libtile2d;

import com.sun.jna.Pointer;
import net.darktree.libtile2d.binding.Binding;

public class Window {

    Pointer pointer;

    public Window( String title, int width, int height ) {
        this.pointer = Binding.LIBRARY.window_new(title, width, height);

        if( !Binding.LIBRARY.window_verify(pointer) ) {
            throw new RuntimeException("Failed to create new window!");
        }
    }

    @Override
    protected void finalize() {
        Binding.LIBRARY.window_free(this.pointer);
    }
}
