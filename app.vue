<script setup lang="ts">
import mqtt, { type MqttClient } from 'mqtt'

const client = ref<MqttClient | null>(null)

enum Topics {
  Events = 'events',
  Commands = 'commands',
}

onMounted(() => {
  client.value = mqtt.connect('mqtt://localhost:8080')

  client.value.on('connect', () =>
    client.value?.subscribe(Topics.Events, (err) => {
      if (!err) {
        client.value?.publish(Topics.Events, 'testiiiing')
      }
    })
  )

  client.value.on('message', (topic, message) => {
    // message is Buffer
    console.log(message.toString())
  })
})

const state = ref('00')
// const led = computed(() => state.value.at(0) === '1')
const led = ref(false)
// const sensor = computed(() => state.value.at(1) === '1')
const sensor = ref(false)

// enum Sensors {
//   LED = 0,
//   SENSOR = 1,
// }

const send = (type: 'led' | 'sensor') => {
  switch (type) {
    case 'led':
      led.value = !led.value
      client.value?.publish(
        Topics.Commands,
        `${led.value ? '0' : '1'}${state.value.at(1)}`
      )
      break
    case 'sensor':
      sensor.value = !sensor.value
      client.value?.publish(
        Topics.Commands,
        `${state.value.at(0)}${sensor.value ? '0' : '1'}`
      )
      break
  }
}
</script>
<template>
  <div class="w-screen h-screen content-center">
    <div class="container grid gap-8 sm:grid-cols-2 lg:grid-cols-3">
      <UiCard>
        <UiCardHeader>
          <UiCardTitle class="leading-9">LED</UiCardTitle>
        </UiCardHeader>
        <UiCardContent class="flex gap-2">
          <UiToggle
            class="flex-1"
            variant="outline"
            :pressed="led"
            @update:pressed="() => send('led')"
          >
            {{ led ? 'On' : 'Off' }}
          </UiToggle>
          <div class="size-10 *:m-auto content-center">
            <IconsLightBulbFilled v-if="led" />
            <IconsLightBulb v-else />
          </div>
        </UiCardContent>
      </UiCard>
      <UiCard>
        <UiCardHeader>
          <div class="flex gap-4">
            <UiCardTitle class="flex-1 leading-9">Sensor</UiCardTitle>
            <UiButton
              @mousedown="() => send('sensor')"
              @mouseup="() => send('sensor')"
              size="sm"
            >
              Press
            </UiButton>
          </div>
        </UiCardHeader>
        <UiCardContent class="space-y-4">
          <div class="flex gap-2">
            <UiToggle
              class="flex-1"
              variant="outline"
              :pressed="sensor"
              @update:pressed="() => send('sensor')"
            >
              {{ sensor ? 'On' : 'Off' }}
            </UiToggle>
            <div class="size-10 *:m-auto content-center">
              <IconsLightBulbFilled v-if="sensor" />
              <IconsLightBulb v-else />
            </div>
          </div>
        </UiCardContent>
      </UiCard>
    </div>
  </div>
</template>
