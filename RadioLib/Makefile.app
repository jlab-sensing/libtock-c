# Include the core RadioLib headers
override CPPFLAGS += -isystem$(TOCK_USERLAND_BASE_DIR)/RadioLib/RadioLib/src

# Include the Tock specific headers
override CPPFLAGS += -isystem$(TOCK_USERLAND_BASE_DIR)/RadioLib

# Exceptions
override CPPFLAGS += -fno-rtti
override CPPFLAGS += -fno-exceptions

# Regions
override CPPFLAGS += -DRADIOLIB_LORAWAN_REGION_US915_ENABLE
