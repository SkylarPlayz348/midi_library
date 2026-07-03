#include "midi_helper.h"

#include <stdbool.h>
#include <string.h>

EventString* new_event_string(EventString* event){
	event->event_string_len = 0;
	event->event_string = malloc(sizeof(char) * event->event_string_len);
	return event;
}

void free_event_string(EventString* event){
	free(event->event_string);
	event->event_string = NULL;
}

EventString* add_to_event(EventString* event, const uint8_t* s, size_t size){
	size_t old = event->event_string_len;
	event->event_string_len += size;
	event->event_string = realloc(event->event_string, sizeof(uint8_t) * event->event_string_len);
	memcpy(event->event_string + old, s, size);
	return event;
}

EventString* add_voice_message(EventString* event, uint8_t status, uint8_t channel){
	uint8_t msg = (status & 0xF0) | (channel & 0x0F);
	event = add_to_event(event, &msg, 1);
	return event;
}

EventString* add_sysex_message(EventString* event, uint8_t type){
	event = add_to_event(event, &type, 1);
	return event;
}

EventString* add_meta_message(EventString* event, uint8_t subtype){
	uint8_t msg[] = {0xFF, subtype};
	event = add_to_event(event, msg, 2);
	return event;
}

EventString* add_string(EventString* event, char* str, size_t s){
	return add_buffer(event, (uint8_t*)str, s);
}

EventString* add_buffer(EventString* event, uint8_t* str, size_t s){
	size_t varlen_size;
	uint8_t* varlen = int_to_varlen(s, &varlen_size);
	event = add_to_event(event, varlen, varlen_size);
	event = add_to_event(event, str, s);
	free(varlen);
	varlen = NULL;
	return event;
}

EventString* add_byte(EventString* event, uint8_t b){
	event = add_to_event(event, &b, 1);
	return event;
}

// DO NOT USE IN CURRENT STATE
bool add_voice_event(MidiTrackChunk* track, uint32_t, delta_time, EventString* event, uint8_t status, uint8_t channel){

};

// DO NOT USE IN CURRENT STATE
bool add_sysex_event(MidiTrackChunk* track, uint32_t, delta_time, EventString* event, uint8_t type){

};

// DO NOT USE IN CURRENT STATE
bool add_meta_event(MidiTrackChunk* track, uint32_t, delta_time, EventString* event, uint8_t subtype){
	event = new_event_string(event);
	event = add_meta_message(event, subtype);
	event = add_string(event, "Trumpet", strlen("Trumpet"));
	track_add_event_full(track, 0, event->event_string, event->event_string_len);
	free_event_string(event);
};