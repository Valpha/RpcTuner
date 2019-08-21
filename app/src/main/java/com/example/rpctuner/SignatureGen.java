package com.example.rpctuner;


import java.util.HashMap;

/**
 * @author Valpha
 */
class SignatureGen {

    public static final HashMap<String, String> PRIMITIVES = new HashMap<String, String>();

    static {
        PRIMITIVES.put(Void.class.getName(), "V");
        PRIMITIVES.put(Boolean.class.getName(), "Z");
        PRIMITIVES.put(Byte.class.getName(), "B");
        PRIMITIVES.put(Character.class.getName(), "C");
        PRIMITIVES.put(Short.class.getName(), "S");
        PRIMITIVES.put(Integer.class.getName(), "I");
        PRIMITIVES.put(Long.class.getName(), "J");
        PRIMITIVES.put(Float.class.getName(), "F");
        PRIMITIVES.put(Double.class.getName(), "D");
    }

    public static String getSignature(Class ret, Class... params) {
        StringBuilder builder = new StringBuilder();
        builder.append("(");
        for (Class param : params) {
            builder.append(getSignature(param));
        }
        builder.append(")");
        builder.append(getSignature(ret));
        return builder.toString();
    }

    protected static String getSignature(Class param) {

        StringBuilder builder = new StringBuilder();
        String name;
        if (param.isArray()) {
            name = param.getComponentType().getName();
            builder.append("[");
        } else {
            name = param.getName();
        }

        if (PRIMITIVES.containsKey(name)) {
            builder.append(PRIMITIVES.get(name));
        } else {
            builder.append("L" + name.replace(".", "/") + ";");
        }

        return builder.toString();
    }
}