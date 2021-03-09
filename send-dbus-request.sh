#!/bin/sh

dbus-send --system --type=method_call --print-reply --dest=com.example.ObjectProviderService /com/example/ObjectProviderService com.example.ObjectProviderService.ListAllObjects
