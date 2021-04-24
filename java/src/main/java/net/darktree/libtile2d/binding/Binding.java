package net.darktree.libtile2d.binding;

import com.sun.jna.Native;
import com.sun.jna.Platform;

public class Binding {

    public static final TileLibrary LIBRARY;

    static {
        String location = Platform.isLinux() ? System.getProperty("user.home") + "/libtile2d/libtile2d.so" : "C:/libtile2d/libtile2d.dll";
        LIBRARY = (TileLibrary) Native.loadLibrary(location, TileLibrary.class);
        LIBRARY.engine_init();
    }

    public static void exit() {
        LIBRARY.engine_exit();
    }

}
