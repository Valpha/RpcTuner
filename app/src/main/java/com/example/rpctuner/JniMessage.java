package com.example.rpctuner;

class JniMessage {
    boolean complete;
    byte[] data;

    public JniMessage(boolean complete, byte[] data) {
        this.complete = complete;
        this.data = data;
    }
}
