<script setup lang="ts">
import mqtt, { type MqttClient } from 'mqtt'

const client = ref<MqttClient | null>(null)

enum Topics {
  Events = 'events',
  Commands = 'commands',
}

onMounted(() => {
  client.value = mqtt.connect('mqtt://192.168.187.60:8080')

  client.value.on('connect', () => {
    client.value?.subscribe(Topics.Events, (err) => {
      if (!err) {
        client.value?.publish('patata', 'Hello mqtt')
      }
    })
  })

  client.value.on('message', (topic, message) => {
    // message is Buffer
    console.log(message.toString())
  })
})

const state = ref('00')
const led = computed(() => state.value.at(0) === '1')
const sensor = computed(() => state.value.at(1) === '1')

// enum Sensors {
//   LED = 0,
//   SENSOR = 1,
// }

const status = ref('')
const send = (type: 'led' | 'sensor') => {
  switch (type) {
    case 'led':
      client.value?.publish(
        Topics.Commands,
        `${led.value ? '0' : '1'}${state.value.at(1)}`
      )
      // socket.value?.send(`${led.value ? '0' : '1'}${state.value.at(1)}`)
      break
    case 'sensor':
      client.value?.publish(
        Topics.Commands,
        `${state.value.at(0)}${sensor.value ? '0' : '1'}`
      )
      // socket.value?.send(`${state.value.at(0)}${sensor.value ? '0' : '1'}`)
      break
  }
}
// const send2 = (type: Sensors) => {}
</script>
<template>
  <div class="w-screen h-screen grid place-content-center gap-8">
    <p>{{ status }}</p>
    <UiButton @click="() => send('led')">test</UiButton>
    <div class="min-w-96 space-y-4">
      <div class="flex gap-2">
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
      </div>
      <UiButton
        @mousedown="() => send('sensor')"
        @mouseup="() => send('sensor')"
      >
        Press
      </UiButton>
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
          <IconsLightBulbFilled v-if="led" />
          <IconsLightBulb v-else />
        </div>
      </div>
    </div>
  </div>
</template>
