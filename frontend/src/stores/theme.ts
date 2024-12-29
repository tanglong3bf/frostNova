import { defineStore } from 'pinia'
import { ref } from 'vue'
import { themes, type Theme } from '@/styles/themes'

export const useThemeStore = defineStore('theme', () => {
  const currentTheme = ref<Theme>(themes[0])

  const setTheme = (theme: Theme) => {
    currentTheme.value = theme
    
    const root = document.documentElement
    Object.entries(theme.colors).forEach(([key, value]) => {
      const cssVar = `--${key.replace(/([A-Z])/g, '-$1').toLowerCase()}`
      root.style.setProperty(cssVar, value)
    })

    try {
      localStorage.setItem('theme', JSON.stringify(theme))
    } catch (error) {
      console.error('Error saving theme:', error)
    }
  }

  // 初始化主题
  try {
    const savedTheme = localStorage.getItem('theme')
    if (savedTheme) {
      setTheme(JSON.parse(savedTheme))
    }
  } catch (error) {
    console.error('Error loading theme:', error)
  }

  return {
    currentTheme,
    setTheme
  }
}) 