import { useCommonStore } from '@/stores/common'
import { storeToRefs } from 'pinia'
import { createRouter, createWebHistory } from 'vue-router'

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/',
      redirect: '/home',
      component: () => import('@/views/layout/LayoutView.vue'),
      meta: { requiresAuth: true },
      children: [
        {
          path: '/home',
          component: () => import('@/views/layout/HomeView.vue')
        }
      ]
    },
    {
      path: '/login',
      component: () => import('@/views/login/LoginView.vue'),
      meta: { requiresAuth: false }
    }
  ],
})


router.beforeEach((to) => {
  const commonStore = useCommonStore()
  const { token } = storeToRefs(commonStore)
  // 是否已经登录
  const isAuthenticated = token.value !== ''
  // 目标页面需要登录、当前未登录
  if (to.matched.some(record => record.meta.requiresAuth) && !isAuthenticated) {
    return '/login'
  }
})

export default router
